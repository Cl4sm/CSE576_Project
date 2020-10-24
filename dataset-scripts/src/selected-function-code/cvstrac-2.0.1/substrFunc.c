static void substrFunc(sqlite3_context *context, int argc, sqlite3_value **argv){
  const char *z;
  const char *z2;
  int i;
  int p1, p2, len;
  int pz1, pz2, zlen;
  mbstate_t st;
  z = (const char *)sqlite3_value_text(argv[0]);
  if( z==0 ) return;
  zlen = sqlite3_value_bytes(argv[0]);
  p1 = sqlite3_value_int(argv[1]);
  p2 = sqlite3_value_int(argv[2]);
  memset(&st, 0, sizeof(st));
  z2 = z;
  len = (int)mbsrtowcs(NULL, &z2, 0, &st);
  if( len <= 0 ) {
    sqlite3_result_text(context, "", 0, SQLITE_STATIC);
    return;
  }
  if( p1<0 ){
    p1 += len;
    if( p1<0 ){
      p2 += p1;
      p1 = 0;
    }
  }else if( p1>0 ){
    p1--;
  }
  if( p1+p2>len ){
    p2 = len-p1;
  }
  memset(&st, 0, sizeof(st));
  for( pz1=0; z[pz1]; pz1+=i ){
    if( p1--<=0 )
      break;
    i = (int)mbrtowc(NULL, z+pz1, zlen-pz1, &st);
    if( i<=0 )
      break;
  }
  for( pz2=0; z[pz1+pz2]; pz2+=i ){
    if( p2--<=0 )
      break;
    i = (int)mbrtowc(NULL, z+pz1+pz2, zlen-pz1-pz2, &st);
    if( i<=0 )
      break;
  }
  sqlite3_result_text(context, (char*)&z[pz1], pz2, SQLITE_TRANSIENT);
}