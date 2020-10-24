static char *TokensToString(
  Token *pFirst,    /* First token in the string */
  Token *pLast,     /* Last token in the string */
  char *zTerm,      /* Terminate the string with this text if not NULL */
  Token *pSkip,     /* Skip this token if not NULL */
  int nSkip         /* Skip a total of this many tokens */
){
  char *zReturn;
  String str;
  int needSpace = 0;
  int c;
  int iSkip = 0;
  int skipOne = 0;

  StringInit(&str);
  pLast = pLast->pNext;
  while( pFirst!=pLast ){
    if( pFirst==pSkip ){ iSkip = nSkip; }
    if( iSkip>0 ){ 
      iSkip--;
      pFirst=pFirst->pNext; 
      continue;
    }
    switch( pFirst->eType ){
      case TT_Preprocessor:
        StringAppend(&str,"\n",1);
        StringAppend(&str,pFirst->zText,pFirst->nText);
        StringAppend(&str,"\n",1);
        needSpace = 0;
        break;

      case TT_Id: 
        switch( pFirst->zText[0] ){
          case 'E':        
            if( pFirst->nText==6 && strncmp(pFirst->zText,"EXPORT",6)==0 ){
              skipOne = 1;
            }
            break;
          case 'P':
            switch( pFirst->nText ){
              case 6:  skipOne = !strncmp(pFirst->zText,"PUBLIC", 6);    break;
              case 7:  skipOne = !strncmp(pFirst->zText,"PRIVATE",7);    break;
              case 9:  skipOne = !strncmp(pFirst->zText,"PROTECTED",9);  break;
              default: break;
            }
            break;
          default:
            break;
        }
        if( skipOne ){
          pFirst = pFirst->pNext;
          continue;
        }
        /* Fall thru to the next case */
      case TT_Number:
        if( needSpace ){
          StringAppend(&str," ",1);
        }
        StringAppend(&str,pFirst->zText,pFirst->nText);
        needSpace = 1;
        break;

      default:
        c = pFirst->zText[0];
        if( needSpace && (c=='*' || c=='{') ){
          StringAppend(&str," ",1);
        }
        StringAppend(&str,pFirst->zText,pFirst->nText);
        /* needSpace = pFirst->zText[0]==','; */
        needSpace = 0;
        break;
    }
    pFirst = pFirst->pNext;
  }
  if( zTerm && *zTerm ){
    StringAppend(&str,zTerm,strlen(zTerm));
  }
  zReturn = StrDup(StringGet(&str),0);
  StringReset(&str);
  return zReturn;
}