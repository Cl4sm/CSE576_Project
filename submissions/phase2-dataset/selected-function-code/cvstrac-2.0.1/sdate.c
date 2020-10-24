static void sdate(sqlite3_context *context, int argc, sqlite3_value **argv){
  time_t now;
  time_t t;
  struct tm *pTm;
  char *zFormat;
  char zBuf[200];
  
  if( argc!=1 ) return;
  t = sqlite3_value_int(argv[0]);
  if( t==0 ) return;
  time(&now);
  if( t+8*3600 > now && t-8*3600 <= now ){
    zFormat = "%H:%M";
  }else if( t+24*3600*120 > now && t-24*3600*120 < now ){
    zFormat = "%b %d";
  }else{
    zFormat = "%Y %b";
  }
  pTm = localtime(&t);
  strftime(zBuf, sizeof(zBuf), zFormat, pTm);
  sqlite3_result_text(context, zBuf, -1, SQLITE_TRANSIENT);
}