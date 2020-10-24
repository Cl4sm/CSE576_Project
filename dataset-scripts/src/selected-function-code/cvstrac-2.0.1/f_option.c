static void f_option(sqlite3_context *context, int argc, sqlite3_value **argv){
  const char *zParm;
  int i;
  if( argc!=1 && argc!=2 ) return;

  zParm = (const char*)sqlite3_value_text(argv[0]);
  if( zParm==0 ) return;

  for(i=0; i<g.nAux && g.azAuxName[i]; i++){
    if( strcasecmp(zParm,g.azAuxName[i])==0 ){
      if( g.azAuxVal[i] ){
        sqlite3_result_text(context, g.azAuxVal[i], -1, SQLITE_STATIC);
      }
      return;
    }
  }
  if( argc>1 && g.nAux<MX_AUX ){
    char *zErr;
    char *zQuery = (char *)sqlite3_value_text(argv[1]);
    if( zQuery==0 ) {
      db_err("Query cannot be NULL", "NULL", "Illegal option() query");
    }
    zErr = verify_sql_statement(zQuery);
    if( zErr ){
      db_err( zErr, zQuery, "Illegal option() query" );
    }else{
      int rc;
      struct QueryResult sResult;
      if( pDb==0 ) db_open();
      memset(&sResult, 0, sizeof(sResult));

      db_restrict_query(1);
      rc = sqlite3_exec(pDb, zQuery, db_query_callback, &sResult, &zErr);
      db_restrict_query(0);
      if( rc != SQLITE_OK ){
        db_err( zErr, zQuery, "option() query failed" );
      }
      
      if( sResult.azElem==0 ){
        db_query_callback(&sResult, 0, 0, 0);
      }
      sResult.azElem[sResult.nElem] = 0;

      g.azAuxOpt[g.nAux] = (const char**)sResult.azElem;
      g.anAuxCols[g.nAux] = sResult.nCols;
    }
    g.azAuxName[g.nAux] = mprintf("%s",zParm);
    g.azAuxParam[g.nAux] = mprintf("%s",zParm);
    for(i=0; g.azAuxParam[g.nAux][i]; i++){
      if(!isalnum(g.azAuxParam[g.nAux][i])) g.azAuxParam[g.nAux][i]='_';
    }
    g.azAuxVal[g.nAux] = PD(g.azAuxParam[g.nAux],
                            g.azAuxOpt[g.nAux][0]?g.azAuxOpt[g.nAux][0]:"");
    if( g.azAuxVal[g.nAux] ){
      sqlite3_result_text(context, g.azAuxVal[g.nAux], -1, SQLITE_STATIC);
    }
    g.nAux++;
  }
}