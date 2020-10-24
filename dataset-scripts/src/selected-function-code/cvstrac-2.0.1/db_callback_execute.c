void db_callback_execute(
  int (*xCallback)(void*,int,char**,char**),  /* Callback for each row */
  void *pArg,                                 /* 1st argument to callback */
  const char *zFormat,                        /* SQL for the query */
  ...                                         /* Arguments for the SQL */
){
  int rc;
  char *zErrMsg = 0;
  char *zSql;
  va_list ap;

  if( pDb==0 ) db_open();
  va_start(ap, zFormat);
  zSql = sqlite3_vmprintf(zFormat, ap);
  va_end(ap);
  rc = sqlite3_exec(pDb, zSql, xCallback, pArg, &zErrMsg);
  if( rc != SQLITE_OK ){
    db_err(zErrMsg ? zErrMsg : sqlite3_errmsg(pDb), zSql,
           "db_callback_execute: Database query failed");
  }
  sqlite3_free(zSql);
}