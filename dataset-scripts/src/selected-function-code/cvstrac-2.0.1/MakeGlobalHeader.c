static int MakeGlobalHeader(int forExport){
  GenState sState;
  String outStr;
  IdentTable includeTable;
  Decl *pDecl;

  sState.pStr = &outStr;
  StringInit(&outStr);
  /* StringAppend(&outStr,zTopLine,nTopLine); */
  sState.pTable = &includeTable;
  memset(&includeTable,0,sizeof(includeTable));
  sState.zIf = 0;
  sState.nErr = 0;
  sState.zFilename = "(all)";
  sState.flags = 0;
  ResetDeclFlags(0);
  for(pDecl=pDeclFirst; pDecl; pDecl=pDecl->pNext){
    if( forExport==0 || DeclHasProperty(pDecl,DP_Export) ){
      DeclareObject(pDecl,&sState,1);
    }
  }
  ChangeIfContext(0,&sState);
  printf("%s",StringGet(&outStr));
  IdentTableReset(&includeTable);
  StringReset(&outStr);
  return 0;  
}