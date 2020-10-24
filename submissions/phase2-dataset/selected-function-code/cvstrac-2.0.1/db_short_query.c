char *db_short_query(const char *zFormat, ...){
  va_list ap;
  int rc;
  char *zResult = 0;
  char *zErrMsg = 0;
  char *zSql;

  assert(zFormat);
  if( pDb==0 ) db_open();
  va_start(ap, zFormat);
  zSql = sqlite3_vmprintf(zFormat,ap);
  va_end(ap);
  db_restrict_query(1);
  rc = sqlite3_exec(pDb, zSql, db_short_query_callback, &zResult, &zErrMsg);
  db_restrict_query(0);

  /* short query callback aborts when we get a real value */
  if( rc != SQLITE_OK && rc != SQLITE_ABORT ){
    db_err( zErrMsg ? zErrMsg : sqlite3_errmsg(pDb), zSql,
            "db_short_query: Database query failed" );
  }
  sqlite3_free(zSql);
  return zResult;
}