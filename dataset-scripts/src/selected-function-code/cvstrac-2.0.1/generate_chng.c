static void generate_chng(sqlite3 *db, time_t when, int cn){
  int rc;
  char *zSql;
  char *zErrMsg;
  char zDir[4096];
  char zBase[4096];
  int depth = 1+(rand()%MAX_DEPTH);
  int num_filechng = 1+(rand()%MAX_FILECHNG);
  int j,k;

  zDir[0] = 0;

  sqlite3_exec(db,"BEGIN;",0,0,&zErrMsg);

  /* For simplicity, each CHNG record only hits a single directory. */
  for(j=k=0;j<depth;j++){
    sprintf(zBase,"dir_%d",rand()%MAX_DIR);
    zSql = sqlite3_mprintf("REPLACE INTO file VALUES(1,'%q','%q')",zBase,zDir);
    sqlite3_exec(db,zSql,0,0,&zErrMsg);
    sqlite3_free(zSql);
    if(zDir[0]) strcat(zDir,"/");
    strcat(zDir,zBase);
  }

  /* FILECHNG records need to be unique for any given cn, so we
  ** fudge things a touch here
  */
  for(j=0,k=rand()%10; j<num_filechng; j++,k+=1+(rand()%3)){
    sprintf(zBase,"file_%d.c",k);
    zSql = sqlite3_mprintf("REPLACE INTO file VALUES(0,'%q','%q')",zBase,zDir);
    sqlite3_exec(db,zSql,0,0,&zErrMsg);
    sqlite3_free(zSql);

    /* FIXME: this is definitely _not_ generating CVS version numbers */
    zSql = sqlite3_mprintf("INSERT INTO filechng "
                           "VALUES(%d,'%q/%q','%d',%d,%d)",
                           cn,zDir,zBase,cn,rand()%10,rand()%10);
    sqlite3_exec(db,zSql,0,0,&zErrMsg);
    sqlite3_free(zSql);
  }

  zSql = sqlite3_mprintf("INSERT INTO chng "
                       "VALUES(%d,%d,'',0,'setup','checkin %d of %d');"
                       "COMMIT",
                       cn, when, cn, MAX_CHNG);
  rc = sqlite3_exec(db,zSql,0,0,&zErrMsg);
  sqlite3_free(zSql);
}