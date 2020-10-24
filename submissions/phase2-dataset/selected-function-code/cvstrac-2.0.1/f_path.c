static void f_path(sqlite3_context *context, int argc, sqlite3_value **argv){
  char *zPath;
  const char *zDir;
  if( argc!=3 ) return;
  if( SQLITE_NULL==sqlite3_value_type(argv[0])
                 || SQLITE_NULL==sqlite3_value_type(argv[1])
                 || SQLITE_NULL==sqlite3_value_type(argv[2])){
    return;
  }
  zDir = (const char*)sqlite3_value_text(argv[1]);
  if( 0==sqlite3_value_int(argv[0]) ){
    if( zDir && strlen(zDir)>0 ){
      zPath = mprintf("%s/%s", zDir, (const char*)sqlite3_value_text(argv[2]));
    }else{
      zPath = mprintf("%s", (const char*)sqlite3_value_text(argv[2]));
    }
  }else{
    if( zDir && strlen(zDir)>0 ){
      zPath = mprintf("%s/%s/", zDir, (const char*)sqlite3_value_text(argv[2]));
    }else{
      zPath = mprintf("%s/", (const char*)sqlite3_value_text(argv[2]));
    }
  }
  sqlite3_result_text(context, zPath, -1, SQLITE_TRANSIENT);
  free(zPath);
}