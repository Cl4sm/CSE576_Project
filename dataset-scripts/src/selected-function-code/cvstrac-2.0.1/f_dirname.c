static void f_dirname(sqlite3_context *context, int argc, sqlite3_value **argv){
  const char *z;
  char *zDir;
  int i;
  if( argc!=1 ) return;
  z = (const char*)sqlite3_value_text(argv[0]);
  if( z==0 || z[0]==0 ) return;
  i = strlen(z)-1;
  if( i<=0 ){
    sqlite3_result_text(context, "", -1, SQLITE_TRANSIENT);
    return;
  }
  zDir = mprintf("%s", z);
  if( z[i]=='/' ){ i--; } /* We need to handle dirs too*/
  while( i>=0 && zDir[i]!='/' ){ i--; }
  if( i==0 ){
    zDir = mprintf("");
  }else{
    zDir[i+1] = 0;
  }
  sqlite3_result_text(context, zDir, -1, SQLITE_TRANSIENT);
  free(zDir);
}