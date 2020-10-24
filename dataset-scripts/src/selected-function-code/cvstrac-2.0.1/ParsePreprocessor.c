static int ParsePreprocessor(Token *pToken, int flags, int *pPresetFlags){
  const char *zCmd;
  int nCmd;
  const char *zArg;
  int nArg;
  int nErr = 0;
  Ifmacro *pIf;

  zCmd = &pToken->zText[1];
  while( isspace(*zCmd) && *zCmd!='\n' ){
    zCmd++;
  }
  if( !isalpha(*zCmd) ){
    return 0;
  }
  nCmd = 1;
  while( isalpha(zCmd[nCmd]) ){
    nCmd++;
  }

  if( nCmd==5 && strncmp(zCmd,"endif",5)==0 ){
    /*
    ** Pop the if stack 
    */
    pIf = ifStack;
    if( pIf==0 ){
      fprintf(stderr,"%s:%d: extra '#endif'.\n",zFilename,pToken->nLine);
      return 1;
    }
    ifStack = pIf->pNext;
    SafeFree(pIf);
  }else if( nCmd==6 && strncmp(zCmd,"define",6)==0 ){
    /*
    ** Record a #define if we are in PS_Interface or PS_Export 
    */
    Decl *pDecl;
    if( !(flags & (PS_Local|PS_Interface|PS_Export)) ){ return 0; }
    zArg = &zCmd[6];
    while( *zArg && isspace(*zArg) && *zArg!='\n' ){
      zArg++;
    }
    if( *zArg==0 || *zArg=='\n' ){ return 0; }
    for(nArg=0; ISALNUM(zArg[nArg]); nArg++){}
    if( nArg==0 ){ return 0; }
    pDecl = CreateDecl(zArg,nArg);
    pDecl->pComment = pToken->pComment;
    DeclSetProperty(pDecl,TY_Macro);
    pDecl->zDecl = SafeMalloc( pToken->nText + 2 );
    sprintf(pDecl->zDecl,"%.*s\n",pToken->nText,pToken->zText);
    if( flags & PS_Export ){
      DeclSetProperty(pDecl,DP_Export);
    }else if( flags & PS_Local ){
      DeclSetProperty(pDecl,DP_Local);
    }
  }else if( nCmd==7 && strncmp(zCmd,"include",7)==0 ){
    /*
    ** Record an #include if we are in PS_Interface or PS_Export 
    */
    Include *pInclude;
    char *zIf;

    if( !(flags & (PS_Interface|PS_Export)) ){ return 0; }
    zArg = &zCmd[7];
    while( *zArg && isspace(*zArg) ){ zArg++; }
    for(nArg=0; !isspace(zArg[nArg]); nArg++){}
    if( (zArg[0]=='"' && zArg[nArg-1]!='"')
      ||(zArg[0]=='<' && zArg[nArg-1]!='>')
    ){
      fprintf(stderr,"%s:%d: malformed #include statement.\n",
        zFilename,pToken->nLine);
      return 1;
    }
    zIf = GetIfString();
    if( zIf ){
      pInclude = SafeMalloc( sizeof(Include) + nArg*2 + strlen(zIf) + 10 );
      pInclude->zFile = (char*)&pInclude[1];
      pInclude->zLabel = &pInclude->zFile[nArg+1];
      sprintf(pInclude->zFile,"%.*s",nArg,zArg);
      sprintf(pInclude->zLabel,"%.*s:%s",nArg,zArg,zIf);
      pInclude->zIf = &pInclude->zLabel[nArg+1];
      SafeFree(zIf);
    }else{
      pInclude = SafeMalloc( sizeof(Include) + nArg + 1 );
      pInclude->zFile = (char*)&pInclude[1];
      sprintf(pInclude->zFile,"%.*s",nArg,zArg);
      pInclude->zIf = 0;
      pInclude->zLabel = pInclude->zFile;
    }
    pInclude->pNext = includeList;
    includeList = pInclude;
  }else if( nCmd==2 && strncmp(zCmd,"if",2)==0 ){
    /*
    ** Push an #if.  Watch for the special cases of INTERFACE
    ** and EXPORT_INTERFACE and LOCAL_INTERFACE
    */
    zArg = &zCmd[2];
    while( *zArg && isspace(*zArg) && *zArg!='\n' ){
      zArg++;
    }
    if( *zArg==0 || *zArg=='\n' ){ return 0; }
    nArg = pToken->nText + (int)pToken->zText - (int)zArg;
    if( nArg==9 && strncmp(zArg,"INTERFACE",9)==0 ){
      PushIfMacro(0,0,0,pToken->nLine,PS_Interface);
    }else if( nArg==16 && strncmp(zArg,"EXPORT_INTERFACE",16)==0 ){
      PushIfMacro(0,0,0,pToken->nLine,PS_Export);
    }else if( nArg==15 && strncmp(zArg,"LOCAL_INTERFACE",15)==0 ){
      PushIfMacro(0,0,0,pToken->nLine,PS_Local);
    }else{
      PushIfMacro(0,zArg,nArg,pToken->nLine,0);
    }
  }else if( nCmd==5 && strncmp(zCmd,"ifdef",5)==0 ){
    /* 
    ** Push an #ifdef.
    */
    zArg = &zCmd[5];
    while( *zArg && isspace(*zArg) && *zArg!='\n' ){
      zArg++;
    }
    if( *zArg==0 || *zArg=='\n' ){ return 0; }
    nArg = pToken->nText + (int)pToken->zText - (int)zArg;
    PushIfMacro("defined",zArg,nArg,pToken->nLine,0);
  }else if( nCmd==6 && strncmp(zCmd,"ifndef",6)==0 ){
    /*
    ** Push an #ifndef.
    */
    zArg = &zCmd[6];
    while( *zArg && isspace(*zArg) && *zArg!='\n' ){
      zArg++;
    }
    if( *zArg==0 || *zArg=='\n' ){ return 0; }
    nArg = pToken->nText + (int)pToken->zText - (int)zArg;
    PushIfMacro("!defined",zArg,nArg,pToken->nLine,0);
  }else if( nCmd==4 && strncmp(zCmd,"else",4)==0 ){
    /*
    ** Invert the #if on the top of the stack 
    */
    if( ifStack==0 ){
      fprintf(stderr,"%s:%d: '#else' without an '#if'\n",zFilename,
         pToken->nLine);
      return 1;
    }
    pIf = ifStack;
    if( pIf->zCondition ){
      ifStack = ifStack->pNext;
      PushIfMacro("!",pIf->zCondition,strlen(pIf->zCondition),pIf->nLine,0);
      SafeFree(pIf);
    }else{
      pIf->flags = 0;
    }
  }else{
    /*
    ** This directive can be safely ignored 
    */
    return 0;
  }

  /* 
  ** Recompute the preset flags 
  */
  *pPresetFlags = 0;
  for(pIf = ifStack; pIf; pIf=pIf->pNext){
    *pPresetFlags |= pIf->flags;
  }
    
  return nErr;
}