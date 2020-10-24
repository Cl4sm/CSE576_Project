char *mprintf(const char *zFormat, ...){
  va_list ap;
  struct sgMprintf sMprintf;
  char *zNew;
  char zBuf[200];

  sMprintf.nChar = 0;
  sMprintf.nAlloc = sizeof(zBuf);
  sMprintf.zText = zBuf;
  sMprintf.zBase = zBuf;
  va_start(ap,zFormat);
  vxprintf(mout,&sMprintf,zFormat,ap);
  va_end(ap);
  sMprintf.zText[sMprintf.nChar] = 0;
  if( sMprintf.zText==sMprintf.zBase ){
    zNew = malloc( sMprintf.nChar+1 );
    if( zNew ) memcpy(zNew, zBuf, sMprintf.nChar+1);
  }else{
    zNew = realloc(sMprintf.zText,sMprintf.nChar+1);
    if( zNew==0 ){
      free(sMprintf.zText);
    }
  }
  if( zNew==0 ) exit(1);
  return zNew;
}