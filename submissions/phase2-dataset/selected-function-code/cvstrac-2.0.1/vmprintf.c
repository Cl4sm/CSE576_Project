char *vmprintf(const char *zFormat, va_list ap){
  struct sgMprintf sMprintf;
  char zBuf[200];
  sMprintf.nChar = 0;
  sMprintf.zText = zBuf;
  sMprintf.nAlloc = sizeof(zBuf);
  sMprintf.zBase = zBuf;
  vxprintf(mout,&sMprintf,zFormat,ap);
  sMprintf.zText[sMprintf.nChar] = 0;
  if( sMprintf.zText==sMprintf.zBase ){
    sMprintf.zText = malloc( sMprintf.nChar+1 );
    if( sMprintf.zText ) memcpy(sMprintf.zText, zBuf, sMprintf.nChar+1);
  }else{
    char *z = realloc(sMprintf.zText,sMprintf.nChar+1);
    if( z==0 ){
      free(sMprintf.zText);
    }
    sMprintf.zText = z;
  }
  if( sMprintf.zText==0 ) exit(1);
  return sMprintf.zText;
}