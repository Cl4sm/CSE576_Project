static void mout(void *arg, char *zNewText, int nNewChar){
  struct sgMprintf *pM = (struct sgMprintf*)arg;
  if( pM->nChar + nNewChar + 1 > pM->nAlloc ){
    pM->nAlloc = pM->nChar + nNewChar*2 + 1;
    if( pM->zText==pM->zBase ){
      pM->zText = malloc(pM->nAlloc);
      if( pM->zText && pM->nChar ) memcpy(pM->zText,pM->zBase,pM->nChar);
    }else{
      char *z = realloc(pM->zText, pM->nAlloc);
      if( z==0 ){
        free(pM->zText);
        pM->nChar = 0;
        pM->nAlloc = 0;
      }
      pM->zText = z;
    }
  }
  if( pM->zText ){
    memcpy(&pM->zText[pM->nChar], zNewText, nNewChar);
    pM->nChar += nNewChar;
    pM->zText[pM->nChar] = 0;
  }
}