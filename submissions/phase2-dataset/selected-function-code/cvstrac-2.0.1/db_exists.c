int db_exists(const char *zFormat, ...){
  va_list ap;
  int iResult = 0;
  char *zErrMsg = 0;
  char *zSql;
  int rc;

  assert(zFormat);
  if( pDb==0 ) db_open();
  va_start(ap, zFormat);
  zSql = sqlite3_vmprintf(zFormat,ap);
  va_end(ap);
  db_restrict_query(1);
  rc = sqlite3_exec(pDb, zSql, db_exists_callback, &iResult, &zErrMsg);
  db_restrict_query(0);
  if( rc!=SQLITE_OK ){
    db_err(zErrMsg, zSql, "db_exists: Database exists query failed");
  }
  sqlite3_free(zSql);
  return iResult;
}