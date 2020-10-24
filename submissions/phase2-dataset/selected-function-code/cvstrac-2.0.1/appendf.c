int appendf(char* zBuf, int* nCurlen, int nBuflen, const char *zFormat, ...){
  va_list ap;
  struct sgMprintf sMprintf;

  if( nBuflen <= 0 ) return -1;

  sMprintf.nChar = nCurlen ? *nCurlen : strlen(zBuf);
  sMprintf.nAlloc = nBuflen;
  sMprintf.zText = zBuf;

  if( sMprintf.nChar<nBuflen ){
    /* bout() NUL terminates... _if_ it's called */
    sMprintf.zText[sMprintf.nChar] = 0;
  }

  va_start(ap,zFormat);
  vxprintf(bout,&sMprintf,zFormat,ap);
  va_end(ap);

  if( nCurlen ) *nCurlen = sMprintf.nChar;
  return sMprintf.nChar;
}