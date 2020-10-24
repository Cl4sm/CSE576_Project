static int ProcessInlineProc(Token *pFirst, int flags, int *pReset){
  Token *pName;
  Token *pEnd;
  Decl *pDecl;

  for(pEnd=pFirst; pEnd; pEnd = pEnd->pNext){
    if( pEnd->zText[0]=='{' || pEnd->zText[0]==';' ){
      *pReset = pEnd->zText[0];
      break;
    }
  }
  if( pEnd==0 ){
    *pReset = ';';
    fprintf(stderr,"%s:%d: incomplete inline procedure definition\n",
      zFilename, pFirst->nLine);
    return 1;
  }
  pName = FindDeclName(pFirst,pEnd);
  if( pName==0 ){
    fprintf(stderr,"%s:%d: malformed inline procedure definition\n",
      zFilename, pFirst->nLine);
    return 1;
  }

#ifdef DEBUG
  if( debugMask & PARSER ){
    printf("**** Found inline routine: %.*s on line %d...\n", 
       pName->nText, pName->zText, pFirst->nLine);
    PrintTokens(pFirst,pEnd);
    printf("\n");
  }
#endif
  pDecl = CreateDecl(pName->zText,pName->nText);
  pDecl->pComment = pFirst->pComment;
  DeclSetProperty(pDecl,TY_Subroutine);
  pDecl->zDecl = TokensToString(pFirst,pEnd,";\n",0,0);
  if( (flags & (PS_Static|PS_Local|PS_Local2)) ){
    DeclSetProperty(pDecl,DP_Local);
  }else if( flags & (PS_Export|PS_Export2) ){
    DeclSetProperty(pDecl,DP_Export);
  }

  if( flags & DP_Cplusplus ){
    DeclSetProperty(pDecl,DP_Cplusplus);
  }else{
    DeclSetProperty(pDecl,DP_ExternCReqd);
  }

  return 0;
}