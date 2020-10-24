static int GetNonspaceToken(InStream *pIn, Token *pToken){
  int nIf = 0;
  int inZero = 0;
  const char *z;
  int value;
  int startLine;
  int nErr = 0;

  startLine = pIn->nLine;
  while( 1 ){
    nErr += GetToken(pIn,pToken);
    /* printf("%04d: Type=%d nIf=%d [%.*s]\n",
       pToken->nLine,pToken->eType,nIf,pToken->nText,
       pToken->eType!=TT_Space ? pToken->zText : "<space>"); */
    pToken->pComment = blockComment;
    switch( pToken->eType ){
      case TT_Comment:
      case TT_Space:
        break;

      case TT_BlockComment:
        if( doc_flag ){
          blockComment = SafeMalloc( sizeof(Token) );
          *blockComment = *pToken;
        }
        break;

      case TT_EOF:
        if( nIf ){
          fprintf(stderr,"%s:%d: Unterminated \"#if\"\n",
             zFilename, startLine);
          nErr++;
        }
        return nErr;

      case TT_Preprocessor:
        z = &pToken->zText[1];
        while( *z==' ' || *z=='\t' ) z++;
        if( sscanf(z,"if %d",&value)==1 && value==0 ){
          nIf++;
          inZero = 1;
        }else if( inZero ){
          if( strncmp(z,"if",2)==0 ){
            nIf++;
          }else if( strncmp(z,"endif",5)==0 ){
            nIf--;
            if( nIf==0 ) inZero = 0;
          }
        }else{
          return nErr;
        }
        break;

      default:
        if( !inZero ){
          return nErr;
        }
        break;
    }
  }
  /* NOT REACHED */
}