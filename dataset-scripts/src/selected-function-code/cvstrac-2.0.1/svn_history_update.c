static int svn_history_update(int isReread){
  const char *zRoot;
  char **azResult;
  FILE *in;
  int nRev;         /* Current revison number in loop */
  int cnum = 0;     /* check-in number to use for this checkin */
  int next_cnum;    /* next unused check-in number */
  int nErr = 0;
  char *zCmd;
  char zLine[2000];
  int nBaseRevision = 0;  /* Revision we last seen, and have stored in db */
  int nHeadRevision = 0;  /* Latest revision in repository */
  int nLine;
  int isInitialScan; /* If set delay updating FILE till after the last revision */
  Revision *pRev;

  db_execute("BEGIN");

  /* Get the path to local repository and last revision number we have in db
   * If there's no repository defined, bail and wait until the admin sets one.
  */
  zRoot = db_config("cvsroot","");
  if( zRoot[0]==0 ) return 1;
  nBaseRevision = atoi(db_config("historysize","0"));

  if( nBaseRevision
      && !svn_did_repository_change(zRoot,atoi(db_config("svnlastupdate","0"))) ){
    db_execute("COMMIT");
    return 1;
  }
  
  isInitialScan = (nBaseRevision==0);
  
  /* Get the number of latest revision in repository
  */
  zCmd = mprintf("svnlook youngest '%s' 2>/dev/null", quotable_string(zRoot));
  
  in = popen(zCmd, "r");
  if( in==0 ){
    error_init(&nErr);
    @ <li><p>Unable to execute the following command:
    @ <blockquote><pre>
    @ %h(zCmd)
    @ </pre></blockquote></p></li>
    error_finish(nErr);
    free(zCmd);
    return -1;
  }
  free(zCmd);
  
  if( fgets(zLine, sizeof(zLine), in) ){
    nHeadRevision = atoi(zLine);
  }
  pclose(in);
  
  /* This could mean that repository is empty, no revisions in it yet.
  ** But this could also mean that atoi() failed. And that would be bad since
  ** it means svnlook's output format changed.
  */
  if( nHeadRevision==0 ){
    error_init(&nErr);
    @ <li><p>Repository '%h(zRoot)' appears empty. Latest revision
    @ seems to be '%h(zLine)'</p></li>
    error_finish(nErr);
    return -1;
  }
  
  /* See if there are some new revisions in repository
  */
  if( nHeadRevision==nBaseRevision ) { 
    /* No changes to the repository since our last scan
    ** Exit without doing anything 
    */
    db_execute("UPDATE config SET value=%d WHERE name='svnlastupdate';", time(NULL)+1);
    db_execute("COMMIT");
    return 0;
  }
  
  /* Find the next available change number
  */
  azResult = db_query("SELECT max(cn) FROM chng");
  next_cnum = atoi(azResult[0])+1;
  db_query_free(azResult);
  
  pRev = (Revision *) malloc( sizeof(*pRev) );

  /*
  ** Parse output of svnlook changes and svnlook info to get the info we need
  ** We do this for each revision we miss from db
  */
  for(nRev=nBaseRevision+1; nRev<=nHeadRevision; nRev++){
    int nAddChng = 1;
    int isMsgEnd = 0;
    /* Example output of "svnlook info" is as follows:
    ** 
    ** chorlya                                      <-Author
    ** 2005-08-02 01:51:45 +0200 (Tue, 02 Aug 2005) <-Date
    ** 26                                           <- # of chars in message
    ** Changes to some make files                   <- multiline message
    **                                              <- followed by blank line
    */
    zCmd = mprintf("svnlook info -r %d '%s' 2>/dev/null", 
                   nRev, quotable_string(zRoot));
    in = popen(zCmd, "r");
    if( in==0 ){
      error_init(&nErr);
      @ <li><p>Unable to execute the following command:
      @ <blockquote><pre>
      @ %h(zCmd)
      @ </pre></blockquote></p></li>
      error_finish(nErr);
      free(zCmd);
      return -1;
    }
    free(zCmd);
    
    nLine = 0;
    while( fgets(zLine,sizeof(zLine),in) ){
      if( nLine==0 ){
        remove_newline(zLine);
        bprintf(pRev->zAuthor, sizeof(pRev->zAuthor), "%.90s", zLine);
      }else if( nLine==1 ){
        struct tm sTm;
        
        /* Since timestamp is localtime, we need to subtract timezone offset 
        ** to get timestamp in UTC
        */
        int nOffstHr, nOffstMn; /* Timezone offset hour and minute */

        zLine[25] = 0;
        if( sscanf(zLine,"%d-%d-%d %d:%d:%d %3d%2d",
                    &sTm.tm_year, &sTm.tm_mon, &sTm.tm_mday,
                    &sTm.tm_hour, &sTm.tm_min, &sTm.tm_sec,
                    &nOffstHr, &nOffstMn)==8 ){
          sTm.tm_year -= 1900;
          sTm.tm_mon--;
          /* We subtract our timezone offset from tm.tm_sec since tm_sec 
          ** is just added to rest of the timestamp without any calculations 
          ** being performed on it. Because of that tm_sec can be negative!
          */
          sTm.tm_sec -= (nOffstHr>0) ? (nOffstHr*60 + nOffstMn)*60
                                       : (nOffstHr*60 - nOffstMn)*60 ;
          pRev->nDate = mkgmtime(&sTm);
        }
      }else if( nLine==2 ){
        pRev->nMsgLength = atoi(zLine);
        if( pRev->nMsgLength==0 ){
          isMsgEnd = 1;
          pRev->zMessage = "";
          break; /* No comment here */
        }else{
          /* Allocate storage space for comment.
          */
          pRev->zMessage = (char *) malloc(pRev->nMsgLength+16);
          if( pRev->zMessage==NULL ){
            break; /* malloc() failed */
          }
        }
      }else{
        /* Before we begin concatenating lines of comment make sure we start
        ** with empty string.
        */
        if( nLine==3 ){
          pRev->zMessage[0] = 0;
        }
        
        /* Concat comment lines into one string */
        strcat(pRev->zMessage, zLine);
      }
      if( isMsgEnd ) break;
      nLine++;
    }
    pclose(in);
    
    if( isReread ){
      char *zPrior = db_short_query(
        "SELECT cn FROM chng WHERE date=%d AND user='%q'",
        pRev->nDate, pRev->zAuthor
      );
      if( zPrior==0 || (cnum = atoi(zPrior))<=0 ){
        cnum = next_cnum++;
      }else{
        nAddChng = 0;
      }
      if(zPrior) free(zPrior);
    }else{
      cnum = next_cnum++; 
    }
    
    /* We assume there can't ever be an empty commit, so we just INSERT
    ** TODO: check information read from "svnlook info" for assumed pattern.
    **       If it deviates from it too much, abort.
    */
    if( nAddChng ){
      if( pRev->zMessage==0 ) pRev->zMessage = "";
      db_execute(
        "INSERT INTO chng(cn, date, branch, milestone, user, message) "
        "VALUES(%d,%d,'',0,'%q','%q');",
        cnum, pRev->nDate, pRev->zAuthor, pRev->zMessage
      );
      xref_add_checkin_comment(cnum, pRev->zMessage);
    }
    
    /* Now that we've got the common revision info, that is same for every 
    ** file in this revision, we need to get the list of files that changed 
    ** in this revision and populate filechng table with it.
    ** 
    ** Example output of "svnlook changed" is as follows:
    ** A   trunk/vendors/deli/
    ** A   trunk/vendors/deli/chips.txt
    ** A   trunk/vendors/deli/sandwich.txt
    ** A   trunk/vendors/deli/pickle.txt
    ** 
    ** First column is svn update-style status letter.
    ** Then there are 3 columns that are of no intrest to us, 
    ** and then comes the file path.
    */
    zCmd = mprintf("svnlook changed -r %d '%s' 2>/dev/null", 
                   nRev, quotable_string(zRoot));
    in = popen(zCmd, "r");
    if( in==0 ){
      error_init(&nErr);
      @ <li><p>Unable to execute the following command:
      @ <blockquote><pre>
      @ %h(zCmd)
      @ </pre></blockquote></p></li>
      error_finish(nErr);
      free(zCmd);
      return -1;
    }
    free(zCmd);
    
    while( fgets(zLine,sizeof(zLine),in) && zLine[0]!='\n'  && zLine[0]!='\r' ){
      int nChngType = 0;
      const char *zFilename = &zLine[4];
      char *zPrevVersion;
      size_t nPos, nLen = strlen(zLine);
      if( nLen<4 ) continue;

      /*
      ** First char indicates what happend to file/dir in this revision.
      ** Following table shows the meaning of some expected values for first 
      ** char and how we map that to filechng field in database:
      ** 
      ** Char  Meaning                    chngtype
      ** =========================================
      **  U    File updated (modified)       0
      **  A    File added to repository      1
      **  D    File deleted (removed)        2
      **
      ** First strip CR and LF chars from right end of filename
      */
      remove_newline(&zLine[4]);

      if( zLine[0]=='U' ){
        nChngType = 0;
      }else if( zLine[0]=='D' ){
        nChngType = 2;
        /* If dir was deleted we need to delete all files and dirs in it.
        */
        nPos = strlen(zFilename)-1;
        if( zFilename[nPos]=='/' ){
          svn_delete_dir(cnum, nRev, zFilename, isInitialScan);
        }
      }else if( zLine[0]=='A' ){
        /* If this is the first revision there can't be any copies in
        ** repository yet.
        */
        if( nRev>1 ){
          /* TODO: check to make sure dir is actually a copy before calling
          ** svn_insert_copied_files() since this func is pretty slow.
          */
          nPos = strlen(zFilename)-1;
          if( zFilename[nPos]=='/' ){
            
            FILE *history;
            char zHistLine[2000];
            char *zCurrPath = NULL;
            int i=0, nHistErr=0;
            zCmd = mprintf("svnlook history -r %d '%s' '%s' 2>/dev/null", nRev, 
                           quotable_string(zRoot), quotable_string(zFilename));
            history = popen(zCmd, "r");
            if( history==0 ){
              error_init(&nErr);
              @ <li><p>Unable to execute the following command:
              @ <blockquote><pre>
              @ %h(zCmd)
              @ </pre></blockquote></p></li>
              error_finish(nErr);
              free(zCmd);
              return -1;
            }
            free(zCmd);
            
            while( fgets(zHistLine,sizeof(zHistLine),history) && i<4 && !nHistErr ){
              remove_newline(zHistLine);
              switch( i ){
                case 0:
                  if( strncmp(zHistLine, "REVISION   PATH", 15)!=0 ) nHistErr++;
                  break;
                case 1:
                  if( strncmp(zHistLine, "--------   ----", 15)!=0 ) nHistErr++;
                  break;
                case 2:
                  /* We need to store this just so we can compare it
                  ** with next line.
                  */
                  zCurrPath = mprintf("%s", &zHistLine[11]);
                  break;
                case 3:
                  /* If paths changed, this dir was copied.
                  */
                  if( strcmp(zCurrPath, &zHistLine[11])!=0 ){
                    svn_insert_copied_files(cnum, nRev, zCurrPath, 
                      &zHistLine[11], isInitialScan);
                  }
                  break;
              }
              i++;
            }
            pclose(history);
            if( zCurrPath ) free(zCurrPath);
          }
        }
        nChngType = 1;
      }else{
        nChngType = -1; /* TODO: do something smart(tm) here :D */
        continue;
      }
      
      zPrevVersion = db_short_query(
        "SELECT vers FROM filechng WHERE filename='%q' AND cn<%d "
        "ORDER BY cn DESC;",
        zFilename, cnum
      );
      
      db_execute(
        "REPLACE INTO filechng(cn,filename,vers,prevvers,chngtype) "
        "VALUES(%d,'%q',%d,'%q',%d);",
        cnum, zFilename, nRev, zPrevVersion ? zPrevVersion : "", nChngType
      );

      if(zPrevVersion) free(zPrevVersion);
      if( !isInitialScan ) insert_file(zFilename, cnum);
    }
    pclose(in);
  }
  
  free(pRev);
  
  /* We delayed populating FILE till now on initial scan */
  if( isInitialScan ){
    update_file_table_with_lastcn();
  }
  
  /* Update the "historysize" entry so that we know last revision number that 
  ** we have in db, and "svnlastupdate" to keep those calls to svnlook youngest
  ** to minimum.
  */
  db_execute("UPDATE config SET value=%d WHERE name='historysize';", nHeadRevision);
  db_execute("UPDATE config SET value=%d WHERE name='svnlastupdate';", time(NULL)+1);
  db_config(0,0);
  
  /* Commit all changes to the database
  */
  db_execute("COMMIT;");
  error_finish(nErr);
  return nErr ? -1 : 0;
}