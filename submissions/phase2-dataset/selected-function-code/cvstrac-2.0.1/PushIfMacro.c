static void PushIfMacro(
  const char *zPrefix,      /* A prefix, like "define" or "!" */
  const char *zText,        /* The condition */
  int nText,                /* Number of characters in zText */
  int nLine,                /* Line number where this macro occurs */
  int flags                 /* Either 0, PS_Interface, PS_Export or PS_Local */
){
  Ifmacro *pIf;
  int nByte;

  nByte = sizeof(Ifmacro);
  if( zText ){
    if( zPrefix ){
      nByte += strlen(zPrefix) + 2;
    }
    nByte += nText + 1;
  }
  pIf = SafeMalloc( nByte );
  if( zText ){
    pIf->zCondition = (char*)&pIf[1];
    if( zPrefix ){
      sprintf(pIf->zCondition,"%s(%.*s)",zPrefix,nText,zText);
    }else{
      sprintf(pIf->zCondition,"%.*s",nText,zText);
    }
  }else{
    pIf->zCondition = 0;
  }
  pIf->nLine = nLine;
  pIf->flags = flags;
  pIf->pNext = ifStack;
  ifStack = pIf;
}