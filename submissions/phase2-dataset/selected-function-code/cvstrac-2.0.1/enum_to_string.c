static char *enum_to_string(const char *zEnum){
  char **az;
  char *zResult;
  int i, j, nByte;
  int len1, len2, len3;
  int mx1, mx2, mx3;
  int rowCnt;
  az = db_query("SELECT name, value, color FROM enums "
                "WHERE type='%s' ORDER BY idx", zEnum);
  rowCnt = mx1 = mx2 = mx3 = 0;
  for(i=0; az[i]; i+=3){
    len1 = strlen(az[i]);
    len2 = strlen(az[i+1]);
    len3 = strlen(az[i+2]);
    if( len1>mx1 ) mx1 = len1;
    if( len2>mx2 ) mx2 = len2;
    if( len3>mx3 ) mx3 = len3;
    rowCnt++;
  }
  if( mx2<mx1 ) mx2 = mx1;
  nByte = (mx1 + mx2 + mx3 + 11)*rowCnt + 1;
  zResult = malloc( nByte );
  if( zResult==0 ) exit(1);
  for(i=j=0; az[i]; i+=3){
    const char *z1 = az[i];
    const char *z2 = az[i+1];
    const char *z3 = az[i+2];
    if( z1[0]==0 ){ z1 = "?"; }
    if( z2[0]==0 ){ z2 = z1; }
    if( z3[0] ){
      bprintf(&zResult[j], nByte-j, "%*s    %*s   (%s)\n",
              -mx1, z1, -mx2, z2, z3);
    }else{
      bprintf(&zResult[j], nByte-j, "%*s    %s\n", -mx1, z1, z2);
    }
    j += strlen(&zResult[j]);
  }
  db_query_free(az);
  zResult[j] = 0;
  return zResult;
}