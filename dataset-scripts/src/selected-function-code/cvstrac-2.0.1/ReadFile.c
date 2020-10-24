static char *ReadFile(const char *zFilename){
  struct stat sStat;
  FILE *pIn;
  char *zBuf;
  int n;

  if( stat(zFilename,&sStat)!=0 
#ifndef WIN32
    || !S_ISREG(sStat.st_mode)
#endif
  ){
    return 0;
  }
  pIn = fopen(zFilename,"r");
  if( pIn==0 ){
    return 0;
  }
  zBuf = SafeMalloc( sStat.st_size + 1 );
  n = fread(zBuf,1,sStat.st_size,pIn);
  zBuf[n] = 0;
  fclose(pIn);
  return zBuf;
}