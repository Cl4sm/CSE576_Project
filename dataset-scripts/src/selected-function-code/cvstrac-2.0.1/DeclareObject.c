static void DeclareObject(
  Decl *pDecl,        /* The thing to be declared */
  GenState *pState,   /* Current state of the code generator */
  int needFullDecl    /* Must have the full declaration.  A forward
                       * declaration isn't enough */
){
  Decl *p;               /* The object to be declared */
  int flag;
  int isCpp;             /* True if generating C++ */
  int doneTypedef = 0;   /* True if a typedef has been done for this object */

  /* printf("BEGIN %s of %s\n",needFullDecl?"FULL":"PROTOTYPE",pDecl->zName);*/
  /* 
  ** For any object that has a forward declaration, go ahead and do the
  ** forward declaration first.
  */
  isCpp = (pState->flags & DP_Cplusplus) != 0;
  for(p=pDecl; p; p=p->pSameName){
    if( p->zFwd ){
      if( !DeclHasProperty(p,DP_Forward) ){
        DeclSetProperty(p,DP_Forward);
        if( strncmp(p->zFwd,"typedef",7)==0 ){
          if( doneTypedef ) continue;
          doneTypedef = 1;
        }
        ChangeIfContext(p->zIf,pState);
        StringAppend(pState->pStr,isCpp ? p->zFwdCpp : p->zFwd,0);
      }
    }
  }

  /*
  ** Early out if everything is already suitably declared.
  **
  ** This is a very important step because it prevents us from
  ** executing the code the follows in a recursive call to this
  ** function with the same value for pDecl.
  */
  flag = needFullDecl ? DP_Declared|DP_Forward : DP_Forward;
  for(p=pDecl; p; p=p->pSameName){
    if( !DeclHasProperty(p,flag) ) break;
  }
  if( p==0 ){
    return;
  }

  /*
  ** Make sure we have all necessary #includes
  */
  for(p=pDecl; p; p=p->pSameName){
    AddIncludes(p->pInclude,pState);
  }

  /*
  ** Go ahead an mark everything as being declared, to prevent an
  ** infinite loop thru the ScanText() function.  At the same time,
  ** we decide which objects need a full declaration and mark them
  ** with the DP_Flag bit.  We are only able to use DP_Flag in this
  ** way because we know we'll never execute this far into this
  ** function on a recursive call with the same pDecl.  Hence, recursive
  ** calls to this function (through ScanText()) can never change the
  ** value of DP_Flag out from under us.
  */
  for(p=pDecl; p; p=p->pSameName){
    if( !DeclHasProperty(p,DP_Declared) 
     && (p->zFwd==0 || needFullDecl) 
     && p->zDecl!=0
    ){
      DeclSetProperty(p,DP_Forward|DP_Declared|DP_Flag);
    }else{
      DeclClearProperty(p,DP_Flag);
    }
  }

  /*
  ** Call ScanText() recusively (this routine is called from ScanText())
  ** to include declarations required to come before these declarations.
  */
  for(p=pDecl; p; p=p->pSameName){
    if( DeclHasProperty(p,DP_Flag) ){
      if( p->zDecl[0]=='#' ){
        ScanText(&p->zDecl[1],pState);
      }else{
        InsertExtraDecl(p);
        ScanText(p->zDecl,pState);
      }
    }
  }

  /*
  ** Output the declarations.  Do this in two passes.  First
  ** output everything that isn't a typedef.  Then go back and
  ** get the typedefs by the same name.
  */
  for(p=pDecl; p; p=p->pSameName){
    if( DeclHasProperty(p,DP_Flag) && !DeclHasProperty(p,TY_Typedef) ){
      if( DeclHasAnyProperty(p,TY_Enumeration) ){
        if( doneTypedef ) continue;
        doneTypedef = 1;
      }
      ChangeIfContext(p->zIf,pState);
      if( !isCpp && DeclHasAnyProperty(p,DP_ExternReqd) ){
        StringAppend(pState->pStr,"extern ",0);
      }else if( isCpp && DeclHasProperty(p,DP_Cplusplus|DP_ExternReqd) ){
        StringAppend(pState->pStr,"extern ",0);
      }else if( isCpp && DeclHasAnyProperty(p,DP_ExternCReqd|DP_ExternReqd) ){
        StringAppend(pState->pStr,"extern \"C\" ",0);
      }
      InsertExtraDecl(p);
      StringAppend(pState->pStr,p->zDecl,0);
      if( !isCpp && DeclHasProperty(p,DP_Cplusplus) ){
        fprintf(stderr,
          "%s: C code ought not reference the C++ object \"%s\"\n",
          pState->zFilename, p->zName);
        pState->nErr++;
      }
      DeclClearProperty(p,DP_Flag);
    }
  }
  for(p=pDecl; p && !doneTypedef; p=p->pSameName){
    if( DeclHasProperty(p,DP_Flag) ){
      /* This has to be a typedef */
      doneTypedef = 1;
      ChangeIfContext(p->zIf,pState);
      InsertExtraDecl(p);
      StringAppend(pState->pStr,p->zDecl,0);
    }
  }
}