static int find_path(
  const char *zPath,       /* The pathname we are looking for */
  void (**pxFunc)(void)    /* Write pointer to handler function here */
){
  int upr, lwr;
  lwr = 0;
  upr = sizeof(aSearch)/sizeof(aSearch[0])-1;
  while( lwr<=upr ){
    int mid, c;
    mid = (upr+lwr)/2;
    c = strcmp(zPath, aSearch[mid].zPath);
    if( c==0 ){
      *pxFunc = aSearch[mid].xFunc;
      return 1;
    }else if( c<0 ){
      upr = mid - 1;
    }else{
      lwr = mid + 1;
    }
  }
  return 0;
}