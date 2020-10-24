char **db_query(const char *zFormat, ...){
  int rc;
  char *zErrMsg = 0;
  va_list ap;
  struct QueryResult sResult;
  char *zSql;

  assert(zFormat!=0);

  va_start(ap, zFormat);
  zSql = sqlite3_vmprintf(zFormat, ap);
  va_end(ap);

  if( pDb==0 ) db_open();
  memset(&sResult, 0, sizeof(sResult));
  db_restrict_query(1);
  rc = sqlite3_exec(pDb, zSql, db_query_callback, &sResult, &zErrMsg);
  db_restrict_query(0);
  if( rc != SQLITE_OK ){
    db_err( zErrMsg ? zErrMsg : sqlite3_errmsg(pDb), zSql,
            "db_query: Database query failed" );
  }
  sqlite3_free(zSql);
  if( sResult.azElem==0 ){
    db_query_callback(&sResult, 0, 0, 0);
  }
  assert(sResult.azElem!=0);
  sResult.azElem[sResult.nElem] = 0;
  return sResult.azElem;
}