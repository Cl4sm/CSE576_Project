void db_add_functions(void){
  if( pDb==0 ) db_open();
  sqlite3_create_function(pDb, "sdate", 1, SQLITE_ANY, 0, &sdate, 0, 0);
  sqlite3_create_function(pDb, "ldate", 1, SQLITE_ANY, 0, &ldate, 0, 0);
  sqlite3_create_function(pDb, "parsedate", 1, SQLITE_ANY, 0, &pdate, 0, 0);
  sqlite3_create_function(pDb, "now", 0, SQLITE_ANY, 0, &f_now, 0, 0);
  sqlite3_create_function(pDb, "user", 0, SQLITE_ANY, 0, &f_user, 0, 0);
  sqlite3_create_function(pDb, "aux", 1, SQLITE_ANY, 0, &f_aux, 0, 0);
  sqlite3_create_function(pDb, "aux", 2, SQLITE_ANY, 0, &f_aux, 0, 0);
  sqlite3_create_function(pDb, "cgi", 1, SQLITE_ANY, 0, &f_cgi, 0, 0);
  sqlite3_create_function(pDb, "cgi", 2, SQLITE_ANY, 0, &f_cgi, 0, 0);
  sqlite3_create_function(pDb, "option", 1, SQLITE_ANY, 0, &f_option, 0, 0);
  sqlite3_create_function(pDb, "option", 2, SQLITE_ANY, 0, &f_option, 0, 0);
  sqlite3_create_function(pDb, "path", 3, SQLITE_ANY, 0, &f_path, 0, 0);
  sqlite3_create_function(pDb, "dirname", 1, SQLITE_ANY, 0, &f_dirname, 0, 0);
  sqlite3_create_function(pDb, "basename", 1, SQLITE_ANY, 0, &f_basename, 0, 0);
  sqlite3_create_function(pDb, "cap_or", 2, SQLITE_ANY, 0, &cap_or, 0, 0);
  sqlite3_create_function(pDb, "cap_and", 2, SQLITE_ANY, 0, &cap_and, 0, 0);
  sqlite3_create_function(pDb, "search", -1, SQLITE_ANY, 0, srchFunc, 0, 0);
#ifdef CVSTRAC_I18N
  if( g.useUTF8==0 ){
    sqlite3_create_function(pDb, "length", 1, SQLITE_ANY, 0, lengthFunc, 0, 0);
    sqlite3_create_function(pDb, "substr", 3, SQLITE_ANY, 0, substrFunc, 0, 0);
  }
#endif
}