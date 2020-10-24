static int GetBigToken(InStream *pIn, Token *pToken, IdentTable *pTable){
  const char *z, *zStart;
  int iStart;
  int nBrace;
  int c;
  int nLine;
  int nErr;

  nErr = GetNonspaceToken(pIn,pToken);
  switch( pToken->eType ){
    case TT_Id:
      if( pTable!=0 ){
        IdentTableInsert(pTable,pToken->zText,pToken->nText);
      }
      return nErr;

    case TT_Preprocessor:
      if( pTable!=0 ){
        FindIdentifiersInMacro(pToken,pTable);
      }
      return nErr;

    case TT_Other:
      if( pToken->zText[0]=='{' ) break;
      return nErr;

    default:
      return nErr;
  }

  z = pIn->z;
  iStart = pIn->i;
  zStart = pToken->zText;
  nLine = pToken->nLine;
  nBrace = 1;
  while( nBrace ){
    nErr += GetNonspaceToken(pIn,pToken);
    /* printf("%04d: nBrace=%d [%.*s]\n",pToken->nLine,nBrace,
       pToken->nText,pToken->zText); */
    switch( pToken->eType ){
      case TT_EOF:
        fprintf(stderr,"%s:%d: Unterminated \"{\"\n",
           zFilename, nLine);
        nErr++;
        pToken->eType = TT_Error;
        return nErr;

      case TT_Id:
        if( pTable ){
          IdentTableInsert(pTable,pToken->zText,pToken->nText);
        }
        break;
  
      case TT_Preprocessor:
        if( pTable!=0 ){
          FindIdentifiersInMacro(pToken,pTable);
        }
        break;

      case TT_Other:
        if( (c = pToken->zText[0])=='{' ){
          nBrace++;
        }else if( c=='}' ){
          nBrace--;
        }
        break;

      default:
        break;
    }
  }
  pToken->eType = TT_Braces;
  pToken->nText = 1 + pIn->i - iStart;
  pToken->zText = zStart;
  pToken->nLine = nLine;
  return nErr;
}