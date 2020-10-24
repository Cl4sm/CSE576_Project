static Token *TokenizeFile(const char *zFile, IdentTable *pTable){
  InStream sIn;
  Token *pFirst = 0, *pLast = 0, *pNew;
  int nErr = 0;

  sIn.z = zFile;
  sIn.i = 0;
  sIn.nLine = 1;
  blockComment = 0;

  while( sIn.z[sIn.i]!=0 ){
    pNew = SafeMalloc( sizeof(Token) );
    nErr += GetBigToken(&sIn,pNew,pTable);
    debug3(TOKENIZER, "Token on line %d: [%.*s]\n",
       pNew->nLine, pNew->nText<50 ? pNew->nText : 50, pNew->zText);
    if( pFirst==0 ){
      pFirst = pLast = pNew;
      pNew->pPrev = 0;
    }else{
      pLast->pNext = pNew;
      pNew->pPrev = pLast;
      pLast = pNew;
    }
    if( pNew->eType==TT_EOF ) break;
  }
  if( pLast ) pLast->pNext = 0;
  blockComment = 0;
  if( nErr ){
    FreeTokenList(pFirst);
    pFirst = 0;
  }

  return pFirst;
}