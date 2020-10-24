static int inSortedList(const char *z, int nCh, const char* azList[], int nList){
  int i;
  int upr, lwr, mid, c;
  char zBuf[32];
  if( nCh<=0 || nCh>sizeof(zBuf)-1 ) return 0;
  for(i=0; i<nCh; i++) zBuf[i] = tolower(z[i]);
  zBuf[i] = 0;
  upr = nList - 1;
  lwr = 0;
  while( upr>=lwr ){
    mid = (upr+lwr)/2;
    c = strcmp(azList[mid],zBuf);
    if( c==0 ) return 1;
    if( c<0 ){
      lwr = mid+1;
    }else{
      upr = mid-1;
    }
  }
  return 0;
}