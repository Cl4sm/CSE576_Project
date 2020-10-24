static char **git_read_commit(
  const char *zGitDir,
  const char *zObject, /* git sha1 object */
  char *zAuthor,       /* at least 100 bytes */
  int *nDate,
  char *zComment,
  int nMaxComment
){
  char *zCmd;
  FILE *in;
  char zLine[PATH_MAX*2];
  int bInMsg = 0;
  char **azParents = 0;
  int nParent = 0;
  int nMaxParent = 0;
  int nComment = 0;
  char zCommitter[100];

  assert(nDate);

  zCmd = mprintf("GIT_DIR='%s' git-cat-file commit '%s' 2>&1",
                 zGitDir, zObject);
  if( zCmd==0 ) return 0;

  in = popen(zCmd, "r");
  if( in==0 ){
    err_pipe("Reading commit",zCmd);
    free(zCmd);
    return 0;
  }
  free(zCmd);

  if( zAuthor ) zAuthor[0] = 0;
  if( zComment ) zComment[0] = 0;
  zCommitter[0] = 0;
  *nDate = 0;

  while( !feof(in) && !ferror(in) ){
    if( 0==fgets(zLine,sizeof(zLine),in) ) break;

    /* you'll get this if it was some other kind of object */
    if( !strncmp(zLine,"error:",6) ) break;

    if( bInMsg==0 ){
      if( zLine[0]=='\n' ){
        bInMsg = 1;
        if( zComment==0 ) break;
      }else if( 0==strncmp(zLine,"parent ",7) ){
        char zParent[100];
        if( nParent+2 >= nMaxParent ){
          nMaxParent = (nParent+2) * 2;
          azParents = realloc(azParents, sizeof(char*)*nMaxParent);
          if( azParents==0 ) common_err("%s",strerror(errno));
        }

        sscanf(&zLine[7],"%50[0-9a-fA-F]",zParent);
        azParents[nParent++] = strdup(zParent);
        azParents[nParent] = 0;

      }else if( zAuthor!= 0 && 0==strncmp(zLine,"author ",7) ){
        sscanf(&zLine[7],"%90[^<]%*[^>]>",zAuthor);
      }else if( 0==strncmp(zLine,"committer ",10) ){
        sscanf(&zLine[10],"%90[^<]%*[^>]> %d",zCommitter,nDate);
      }
    }else{
      int len = strlen(zLine);
      if( len+nComment >= nMaxComment ) break;
      strcpy(&zComment[nComment], zLine);
      nComment += len;
    }
  }
  pclose(in);

  if( *nDate==0 ){
    if( azParents ) db_query_free(azParents);
    return NULL;
  }

  if( zComment && zComment[0]==0 && bInMsg ){
    strncpy(zComment,"Empty log message",nMaxComment);
    nComment = strlen(zComment);
  }

  if( zCommitter[0] ){
    char *zMsg = mprintf( "\n\nCommitter: %s", zCommitter);
    int len = strlen(zLine);
    if( len+nComment < nMaxComment ){
      strcpy(&zComment[nComment], zMsg);
      nComment += len;
    }
    if( zAuthor!=0 && zAuthor[0]==0 ){
      /* apparently GIT commits don't always have an author */
      strcpy(zAuthor, zCommitter);
    }
  }

  return azParents;
}