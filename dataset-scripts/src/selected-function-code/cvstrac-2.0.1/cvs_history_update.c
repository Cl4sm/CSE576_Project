static int cvs_history_update(int isReread){
  int iOldSize;
  const char *zRoot;
  char *zFilename;
  const char *zModule;
  char **azResult;
  char **azFileList;
  FILE *in;
  int i, nField;
  time_t minTime, maxTime, tm;
  struct stat statbuf;
  int cnum = 0;     /* check-in number to use for this checkin */
  int next_cnum;    /* next unused check-in number */
  struct tm *pTm;
  char *zTRange;
  int nErr = 0;
  int path_ok = 0;
  char *azField[20];
  char zLine[2000];
  time_t window = atoi(db_config("checkin_time_window","30"));
  unsigned long t;

  if( window < 30 ) window = 30;
 
  db_execute("BEGIN"); 
  iOldSize = atoi(db_config("historysize","0"));
  zRoot = db_config("cvsroot","");
  zFilename = mprintf("%s/CVSROOT/history", zRoot);
  if( stat(zFilename, &statbuf) || statbuf.st_size==iOldSize ){
    /* The size of the history file has not changed. 
    ** Exit without doing anything */
    db_execute("COMMIT");
    return 0;
  }
  in = fopen(zFilename,"r");
  if( in==0 ){
    error_init(&nErr);
    @ <li><p>Unable to open the history file %h(zFilename).</p></li>
    error_finish(nErr);
    return -1;
  }

  /* The "fc" table records changes to files.  Basically, each line of
  ** the CVSROOT/history file results in one entry in the "fc" table.
  **
  ** The "rev" table holds information about various versions of a particular
  ** file.  The output of the "rlog" command is used to fill in this table.
  */
#if HISTORY_TRACE
  db_execute(
    "CREATE TABLE fc(time,user,file,chngtype,vers);"
    "CREATE TABLE rev(time,ins,del,user,branch,vers,file,comment);"
    "CREATE TABLE dmsg(label,value);"
  );
#else
  db_execute(
    "CREATE TEMP TABLE fc(time,user,file,chngtype,vers text);"
    "CREATE TEMP TABLE rev(time,ins,del,user,branch,vers text,file,comment);"
  );
#endif

  /* Find the next available change number
  */
  azResult = db_query("SELECT max(cn) FROM chng");
  next_cnum = atoi(azResult[0])+1;
  db_query_free(azResult);

  /*
  ** Read the tail of the history file that has not yet been read.
  */
  fseek(in, iOldSize, SEEK_SET);
  minTime = 0x7fffffff;
  maxTime = 0;
  while( fgets(zLine,sizeof(zLine),in) ){
    int c;
    /* The first character of each line tells what the line means:
    **
    **      A    A new file is added to the repository
    **      M    A change is made to an existing file
    **      R    A file is removed
    **      T    Tagging operations
    */
    if( (c = zLine[0])!='A' && c!='M' && c!='T' && c!='R' ) continue;
    if( sscanf(&zLine[1],"%lx",&t)!=1 ) continue;
    tm = (time_t)t;
    if( tm<minTime ) minTime = tm;
    if( tm>maxTime ) maxTime = tm;

    /* Break the line up into fields separated by the '|' character.
    */
    for(i=nField=0; zLine[i]; i++){
      if( zLine[i]=='|' && nField<sizeof(azField)/sizeof(azField[0])-1 ){
        azField[nField++] = &zLine[i+1];
        zLine[i] = 0;
      }
      if( zLine[i]=='\r' || zLine[i]=='\n' ) zLine[i] = 0;
    }

    /* Record all 'A' (add file), 'M' (modify file), and 'R' (removed file)
    ** lines in the "fc" temporary table.
    */
    if( (c=='A' || c=='M' || c=='R') && nField>=5 ){
      db_execute("INSERT INTO fc VALUES(%d,'%q','%q/%q',%d,'%q')",
                 tm, azField[0], azField[2], azField[4],
                 ((c=='A') ? 1 : ((c=='R') ? 2 : 0)),
                 azField[3]);
    }

    /* 'T' lines represent tag creating or deletion.  Construct or modify
    ** corresponding milestones in the database.
    */
    if( zLine[0]=='T' ){
      int date;
      int isDelete;
      struct tm sTm;
      isDelete = azField[2][0]=='D';
      if( azField[2][0]=='A' || azField[2][0]=='D' ){
        date = tm;
      }else if( sscanf(azField[2],"%d.%d.%d.%d.%d.%d",
                  &sTm.tm_year, &sTm.tm_mon, &sTm.tm_mday,
                  &sTm.tm_hour, &sTm.tm_min, &sTm.tm_sec)==6 ){
        sTm.tm_year -= 1900;
        sTm.tm_mon--;
        date = mkgmtime(&sTm);
      }else if(azField[2][0]) {
        /* most likely we're tagging a tag. This may have been done to
        ** turn a tag into a branch, for example. As long as nobody has
        ** editted the message we should be able to grab a date.
        */
        char *z = db_short_query("SELECT date FROM chng "
                                 "WHERE milestone AND message='%q'",
                                 azField[2]);
        if( z==0 ) continue;
        date = atoi(z);
        if( date==0 ) continue;
      }else{
        continue;
      }
      /*
      ** Older db schema's didn't have anywhere to put the directory
      ** information. This meant that an rtag effectively was repository
      ** wide rather than module/directory specific. By deleting these
      ** older tags (with NULL directory entries), we're basically
      ** maintaining the semantics those tags were created under.
      */
      db_execute("DELETE FROM chng WHERE "
                 "milestone=2 AND message='%q' AND "
                 "  (directory ISNULL OR directory='%q');",
                 azField[3],azField[4]);
      if( isDelete ) continue;
      cnum = next_cnum++;
      db_execute("INSERT INTO "
                 "chng(cn,date,branch,milestone,user,message,directory) "
                 "VALUES(%d,%d,'',2,'%q','%q','%q');",
                 cnum, date, azField[0], azField[3], azField[4]);
    }
  }

  /*
  ** Update the "historysize" entry so that we know how much of the history
  ** file has been read.  And close the CVSROOT/history file because we
  ** are finished with it - all the information we need is now in the
  ** "fc" temporary table.
  */
  db_execute("UPDATE config SET value=%d WHERE name='historysize'",
             ftell(in));
  db_config(0,0);
  fclose(in);

  /*
  ** Make sure we recorded at least one file change.  If there were no
  ** file changes in history file, we can stop here.
  */
  if( minTime>maxTime ){
    db_execute("COMMIT");
    return 0;
  }

  /*
  ** If the "module" configuration parameter exists and is not an empty string,
  ** then delete from the FC table all records dealing with files that are
  ** not a part of the specified module.
  */
  zModule = db_config("module", 0);
  if( zModule && zModule[0] ){
    db_execute(
      "DELETE FROM fc WHERE file NOT LIKE '%q%%'",
      zModule
    );
  }

  /*
  ** Extract delta comments from all files that have changed.
  **
  ** For each file that has changed, we run the "rlog" command see all
  ** check-ins that have occurred within an hour of the span of times
  ** that were read from the history file.  This makes sure wee see
  ** all of the check-ins, but it might also see some check-ins that have
  ** already been recorded in the database by a prior run of this procedure.
  ** Those duplicate check-ins will be removed in a subsequent step.
  */
  azFileList = db_query("SELECT DISTINCT file FROM fc");
  minTime -= 3600;
  pTm = gmtime(&minTime);
  strftime(zLine, sizeof(zLine)-1, "%Y-%m-%d %H:%M:%S", pTm);
  i = strlen(zLine);
  strcpy(&zLine[i],"<=");
  i += 2;
  maxTime += 3600;
  pTm = gmtime(&maxTime);
  strftime(&zLine[i], sizeof(zLine)-i-1, "%Y-%m-%d %H:%M:%S", pTm);
  zTRange = mprintf("%s",zLine);
  for(i=0; azFileList[i]; i++){
    char *zCmd;
    char *zFile;
    int nComment;
    int nIns, nDel;
    Br *pBr;
    char *zBr;
    time_t tm;
    int seen_sym = 0;
    int seen_rev = 0;
    char zVers[100];
    char zUser[100];
    char zComment[2000];
    const char *azSubst[10];

    zFile = find_repository_file(zRoot, azFileList[i]);
    if( zFile==0 ){
      error_init(&nErr);
      @ <li><p>Unable to locate the file %h(azFileList[i]) in the
      @ CVS repository</p></li>
      continue;
    }

    /*
    ** Some users might need to run rlog output through a filter for
    ** charset conversions.
    */
    azSubst[0] = "F";
    azSubst[1] = quotable_string(zFile);
    azSubst[2] = "TR";
    azSubst[3] = quotable_string(zTRange);
    azSubst[4] = "RP";
    azSubst[5] = db_config("cvsroot", "");
    azSubst[6] = 0;
    zCmd = subst(db_config("rlog", "rlog '-d%TR' '%F' 2>/dev/null"), azSubst);

    free(zFile);
    HTRACE("zCmd",zCmd);
    in = popen(zCmd, "r");
    if( in==0 ){
      error_init(&nErr);
      @ <li><p>Unable to execute the following command:
      @ <blockquote><pre>
      @ %h(zCmd)
      @ </pre></blockquote></p></li>
      free(zCmd);
      continue;
    }
    nComment = nIns = nDel = 0;
    pBr = 0;
    zBr = "";
    zUser[0] = 0;
    zVers[0] = 0;
    tm = 0;
    while( fgets(zLine, sizeof(zLine), in) ){
      if( strncmp(zLine,"symbolic names:", 14)==0 ){
        /* Lines in the "symbolic names:" section always begin with a tab.
        ** Each line consists of a tab, the name, and a version number.
        ** We are only interested in branch names.  Branch names always contain
        ** a ".0." in the version number.  example:
        **
        **          xyzzy: 1.2.0.3
        **
        ** For each branch, create an instance of a Br structure to record
        ** the version number prefix and name of that branch.
        */
        seen_sym = 1;
        while( fgets(zLine, sizeof(zLine), in) && zLine[0]=='\t' ){
          int i, j;
          char *zV;
          int nDot;
          int isBr;
          Br *pNew;

          /* Find the ':' that separates name from version number */
          for(i=1; zLine[i] && zLine[i]!=':'; i++){}
          if( zLine[i]!=':' ) continue;
          zLine[i] = 0;

          /* Make zV point to the version number */
          zV = &zLine[i+1];
          while( isspace(*zV) ){ zV++; }

          /* Check to see if zV contains ".0." */
          for(i=1, isBr=nDot=0; zV[i] && !isspace(zV[i]); i++){
            if( zV[i]=='.' ){
              nDot++;
              if( zV[i+1]=='0' && zV[i+2]=='.' ) isBr = i;
            }
          }
          if( !isBr || nDot<3 ) continue;  /* Skip the rest of no ".0." */

          /* Remove the ".0." from the version number.  For example,
          ** "1.2.0.3" becomes "1.2.3".
          */
          zV[i] = 0;
          for(i=isBr, j=isBr+2; zV[j]; j++, i++){
            zV[i] = zV[j];
          }
          zV[i++] = '.';
          zV[i] = 0;

          /* Create a Br structure to record this branch name.
          */
          pNew = malloc( sizeof(*pNew) + strlen(&zLine[1]) + i + 2 );
          if( pNew==0 ){
            error_init(&nErr);
            @ <li><p>Out of memory at:
            @ <blockquote><pre>
            @ %h(zLine)
            @ </pre></blockquote></p></li>
            break;
          }
          pNew->pNext = pBr;
          pNew->zVers = (char*)&pNew[1];
          pNew->zName = &pNew->zVers[i+1];
          strcpy(pNew->zVers, zV);
          strcpy(pNew->zName, &zLine[1]);
          pBr = pNew;
        }
      }else if( strncmp(zLine,"revision ", 9)==0 && !zVers[0] ){
        int j;
        Br *p;
        for(j=9; isspace(zLine[j]); j++){}
        strncpy(zVers, &zLine[j], sizeof(zVers)-1);
        zVers[sizeof(zVers)-1] = 0;
        j = strlen(zVers);
        while( j>0 && isspace(zVers[j-1]) ){ j--; }
        zVers[j] = 0;
        nComment = nIns = nDel = 0;
        zUser[0] = 0;
        zBr = "";
        for(p=pBr; p; p=p->pNext){
          int n = strlen(p->zVers);
          if( strncmp(zVers, p->zVers, n)==0 && strchr(&zVers[n],'.')==0 ){
            zBr = p->zName;
            break;
          }
        }
        seen_rev++;
      }else if( strncmp(zLine,"date: ", 6)==0 && zVers[0] && tm==0 ){
        char *z;
        struct tm sTm;
        if( sscanf(&zLine[6],"%d/%d/%d %d:%d:%d",
                  &sTm.tm_year, &sTm.tm_mon, &sTm.tm_mday,
                  &sTm.tm_hour, &sTm.tm_min, &sTm.tm_sec)==6 ){
          sTm.tm_year -= 1900;
          sTm.tm_mon--;
          tm = mkgmtime(&sTm);
        }
        z = strstr(zLine, "author: ");
        if( z ){
          strncpy(zUser, &z[8], sizeof(zUser)-1);
          zUser[sizeof(zUser)-1] = 0;
          z = strchr(zUser,';');
          if( z ) *z = 0;
        }
        z = strstr(zLine, "lines: ");
        if( z ){
          sscanf(&z[7], "%d %d", &nIns, &nDel);
        }
      }else if( strncmp(zLine,"branches: ", 10)==0 ){
        /* Ignore this line */
      }else if( (strncmp(zLine,"-----", 5)==0 || strncmp(zLine,"=====",5)==0)
             && zVers[0] && tm>0 ){
        while( nComment>0 && isspace(zComment[nComment-1]) ){ nComment--; }
        zComment[nComment] = 0;
        db_execute(
          "INSERT INTO rev VALUES(%d,%d,%d,'%q','%q','%s','%q','%q')",
          tm, nIns, -nDel, zUser, zBr, zVers, azFileList[i], zComment
        );
        zVers[0] = 0;
        tm = 0;
        zUser[0] = 0;
      }else if( zVers[0] && zUser[0] ){
        int len = strlen(zLine);
        if( len+nComment >= sizeof(zComment)-1 ){
          len = sizeof(zComment)-nComment-1;
          if( len<0 ) len = 0;
          zLine[len] = 0;
        }
        strcpy(&zComment[nComment], zLine);
        nComment += len;
      }
    }
    while( pBr ){
      Br *pNext = pBr->pNext;
      free(pBr);
      pBr = pNext;
    }
    pclose(in);
    if( seen_rev==0 ){
      error_init(&nErr);
      if( !path_ok ){
        check_path(nErr, "rlog");
        path_ok = 1;
      }
      @ <p><li>No revision information found in <b>rlog</b> output:
      @ <blockquote><pre>
      @ %h(zCmd);
      @ </pre></blockquote></p></li>
    }else if( seen_sym==0 ){
      error_init(&nErr);
      @ <p><li>No "<b>symbolic names:</b>" line seen in <b>rlog</b> output:
      @ <blockquote><pre>
      @ %h(zCmd);
      @ </pre></blockquote></p></li>
    }
    free(zCmd);
  }
  db_query_free(azFileList);

  /* Delete entries from the REV table that already exist in the database.
  ** Note that we have to ensure the revision comparison is string-based and
  ** not numeric.
  */
  db_execute(
    "CREATE INDEX rev_idx1 ON rev(file,vers);"
    "DELETE FROM rev WHERE rowid IN ("
       "SELECT rev.rowid FROM filechng, rev "
       "WHERE filechng.filename=rev.file AND filechng.vers||''=rev.vers||''"
    ");"
  );

  /* Scan through the REV table to construct CHNG and FILECHNG entries
  */
  azResult = db_query(
     "SELECT time, user, branch, vers, ins, del, file, comment FROM rev "
     "ORDER BY time, comment, user, branch"
  );
  for(i=0; azResult[i]; i+=8){
    if(                      /* For each FILECHNG, create a new CHNG if... */
       i==0 ||                                      /* first entry */
       strcmp(azResult[i+7],azResult[i-1])!=0 ||    /* or comment changed */
       strcmp(azResult[i+1],azResult[i-7])!=0 ||    /* or user changed */
       strcmp(azResult[i+2],azResult[i-6])!=0 ||    /* or branch changed */
       atoi(azResult[i])>atoi(azResult[i-8])+window /* or not with n seconds */
    ){
      int add_chng = 1;
      if( isReread ){
        const char *zPrior = db_short_query(
          "SELECT cn FROM chng WHERE date=%d AND user='%q'",
          atoi(azResult[i]), azResult[i+1]
        );
        if( zPrior==0 || (cnum = atoi(zPrior))<=0 ){
          cnum = next_cnum++;
        }else{
          add_chng = 0;
        }
      }else{
        cnum = next_cnum++; 
      }
      if( add_chng ){
        db_execute(
          "INSERT INTO chng(cn, date, branch, milestone, user, message) "
          "VALUES(%d,%d,'%q',0,'%q','%q')",
          cnum, atoi(azResult[i]), azResult[i+2], azResult[i+1], azResult[i+7]
        );
        xref_add_checkin_comment(cnum, azResult[i+7]);
      }
    }

    
    {
      char zVers[100];
      char *az = db_short_query("SELECT chngtype FROM fc WHERE "
                                "file='%q' AND vers='%q'",
                                azResult[i+6], azResult[i+3]);
      int chngtype = az ? atoi(az) : 2;
      if(az) free(az);

      if( chngtype!=1 ){
        /* go figure previous version. */
        /* FIXME: it'd be better to determine this during the rlog or something.
         * This is more brittle than I like. It should be fine for the most part if
         * repository admins don't directly mess with revision numbers, but the
         * prevvers chain gets broken if that happens. So far the effects are
         * benign.
         */
        strncpy(zVers,azResult[i+3],sizeof(zVers));
        cvs_previous_version(zVers,azResult[i+6]);
      }else{
        zVers[0] = 0;
      }

      db_execute(
        "REPLACE INTO filechng(cn,filename,vers,prevvers,chngtype,nins,ndel) "
        "VALUES(%d,'%q','%q','%q',%d,%d,%d)",
        cnum, azResult[i+6], azResult[i+3], zVers, chngtype,
        atoi(azResult[i+4]), atoi(azResult[i+5])
      );
    }
    
    if( iOldSize>0 ) insert_file(azResult[i+6], cnum);
  }
  db_query_free(azResult);
  
  /* We delayed populating FILE till now on initial scan */
  if( iOldSize==0 ){
    update_file_table_with_lastcn();
  }
  
  /* Commit all changes to the database
  */
  db_execute("COMMIT;");
  error_finish(nErr);

  return nErr ? -1 : 0;
}