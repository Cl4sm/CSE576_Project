static int ProcessProcedureDef(Token *pFirst, Token *pLast, int flags){
  Token *pName;
  Decl *pDecl;
  Token *pCode;

  if( pFirst==0 || pLast==0 ){
    return 0;
  }
  if( flags & PS_Method ){
    if( flags & PS_PPP ){
      return ProcessMethodDef(pFirst, pLast, flags);
    }else{
      return 0;
    }
  }
  if( (flags & PS_Static)!=0 && !proto_static ){
    return 0;
  }
  pCode = pLast;
  while( pLast && pLast!=pFirst && pLast->zText[0]!=')' ){
    pLast = pLast->pPrev;
  }
  if( pLast==0 || pLast==pFirst || pFirst->pNext==pLast ){
    fprintf(stderr,"%s:%d: Unrecognized syntax.\n", 
      zFilename, pFirst->nLine);
    return 1;
  }
  if( flags & (PS_Interface|PS_Export|PS_Local) ){
    fprintf(stderr,"%s:%d: Missing \"inline\" on function or procedure.\n",
      zFilename, pFirst->nLine);
    return 1;
  }
  pName = FindDeclName(pFirst,pLast);
  if( pName==0 ){
    fprintf(stderr,"%s:%d: Malformed function or procedure definition.\n",
      zFilename, pFirst->nLine);
    return 1;
  }

  /*
  ** At this point we've isolated a procedure declaration between pFirst
  ** and pLast with the name pName.
  */
#ifdef DEBUG
  if( debugMask & PARSER ){
    printf("**** Found routine: %.*s on line %d...\n", pName->nText,
       pName->zText, pFirst->nLine);
    PrintTokens(pFirst,pLast);
    printf(";\n");
  }
#endif
  pDecl = CreateDecl(pName->zText,pName->nText);
  pDecl->pComment = pFirst->pComment;
  if( pCode && pCode->eType==TT_Braces ){
    pDecl->tokenCode = *pCode;
  }
  DeclSetProperty(pDecl,TY_Subroutine);
  pDecl->zDecl = TokensToString(pFirst,pLast,";\n",0,0);
  if( (flags & (PS_Static|PS_Local2))!=0 ){
    DeclSetProperty(pDecl,DP_Local);
  }else if( (flags & (PS_Export2))!=0 ){
    DeclSetProperty(pDecl,DP_Export);
  }

  if( flags & DP_Cplusplus ){
    DeclSetProperty(pDecl,DP_Cplusplus);
  }else{
    DeclSetProperty(pDecl,DP_ExternCReqd);
  }

  return 0;
}