static void git_ingest_commit_chng(
  const char *zGitDir,
  int cn,
  const char *zCommit,
  time_t nDate,
  const char *zAuthor,
  const char *zComment,
  const char *zPrevVers,
  int skipInsertFile
){
  FILE *in = 0;
  char zLine[PATH_MAX*3];

  if( zPrevVers[0]==0 ){
    /* Initial commit, hence no parent(s) to compare against. That means just a
    ** straight tree list
    */

    char *zCmd = mprintf("GIT_DIR='%s' git-ls-tree -r '%s'", zGitDir, zCommit);
    in = popen(zCmd,"r");
    if( in==0 ){
      err_pipe("Reading tree",zCmd);
      return;
    }
    free(zCmd);

    while( !feof(in) && !ferror(in) ){
      char zMode[100], zType[100], zObject[100], zPath[PATH_MAX];

      if( 0==fgets(zLine,sizeof(zLine),in) ) break;
      remove_newline(zLine);

      sscanf(zLine, "%8[0-9] %90s %50[0-9a-fA-F] %[^\t]",
             zMode, zType, zObject, zPath);

      if( !strcmp(zType,"blob") ){
        int nIns = 0;
        int nDel = 0;

        db_execute(
          "INSERT INTO filechng(cn,filename,vers,prevvers,chngtype,nins,ndel) "
          "VALUES(%d,'%q','%s','',1,%d,%d)",
          cn, zPath, zCommit, nIns, nDel);
        if( !skipInsertFile ) insert_file(zPath, cn);
      }
    }
  }else{
    /* Now get the list of changed files and turn them into FILE
    ** and FILECHNG records.  git-diff-tree is disgustingly PERFECT for
    ** this. Compared to the hassles one has to go through with CVS or
    ** Subversion to find out what's in a change tree, it's just mind
    ** blowing how ideal this is.  FIXME: we're not handling renames or
    ** copies right now. When/if we do, add in the "-C -M" flags.
    */

    char *zCmd = mprintf("GIT_DIR='%s' git-diff-tree -r -t '%s' '%s'",
                         zGitDir, zPrevVers, zCommit);
    in = popen(zCmd,"r");
    if( in==0 ){
      err_pipe("Reading tree",zCmd);
      return;
    }
    free(zCmd);

    while( !feof(in) && !ferror(in) ){
      char zSrcMode[100], zDstMode[100], zSrcObject[100], zDstObject[100];
      char cStatus, zPath[PATH_MAX];

      if( 0==fgets(zLine,sizeof(zLine),in) ) break;
      remove_newline(zLine);

      sscanf(zLine, "%*c%8s %8s %50[0-9a-fA-F] %50[0-9a-fA-F] %c %[^\t]",
             zSrcMode, zDstMode, zSrcObject, zDstObject, &cStatus, zPath);

      if( zSrcMode[1]=='0' || zDstMode[1]=='0' ){
        int nIns = 0;
        int nDel = 0;

        if( cStatus=='N' || cStatus=='A' ){
          if( !skipInsertFile ) insert_file(zPath, cn);
          db_execute(
            "INSERT INTO "
            "       filechng(cn,filename,vers,prevvers,chngtype,nins,ndel) "
            "VALUES(%d,'%q','%s','',1,%d,%d)",
            cn, zPath, zCommit, nIns, nDel);
        }else if( cStatus=='D' ){
          db_execute(
            "INSERT INTO "
            "       filechng(cn,filename,vers,prevvers,chngtype,nins,ndel) "
            "VALUES(%d,'%q','%s','%s',2,%d,%d)",
            cn, zPath, zCommit, zPrevVers, nIns, nDel);
        }else{
          db_execute(
            "INSERT INTO "
            "       filechng(cn,filename,vers,prevvers,chngtype,nins,ndel) "
            "VALUES(%d,'%q','%s','%s',0,%d,%d)",
            cn, zPath, zCommit, zPrevVers, nIns, nDel);
        }
      }
    }
  }
  assert(in);
  pclose(in);

  db_execute(
    "INSERT INTO chng(cn, date, branch, milestone, user, message) "
    "VALUES(%d,%d,'',0,'%q','%q')",
    cn, nDate, zAuthor, zComment
  );
  xref_add_checkin_comment(cn, zComment);
}