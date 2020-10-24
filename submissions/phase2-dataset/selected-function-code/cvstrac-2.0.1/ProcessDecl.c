static int ProcessDecl(Token *pFirst, Token *pEnd, int flags){
  Token *pName;
  Decl *pDecl;
  int isLocal = 0;
  int isVar;
  int nErr = 0;

  if( pFirst==0 || pEnd==0 ){
    return 0;
  }
  if( flags & PS_Typedef ){
    if( (flags & (PS_Export2|PS_Local2))!=0 ){
      fprintf(stderr,"%s:%d: \"EXPORT\" or \"LOCAL\" ignored before typedef.\n",
        zFilename, pFirst->nLine);
      nErr++;
    }
    if( (flags & (PS_Interface|PS_Export|PS_Local|DP_Cplusplus))==0 ){
      /* It is illegal to duplicate a typedef in C (but OK in C++).
      ** So don't record typedefs that aren't within a C++ file or
      ** within #if INTERFACE..#endif */
      return nErr;
    }
    if( (flags & (PS_Interface|PS_Export|PS_Local))==0 && proto_static==0 ){
      /* Ignore typedefs that are not with "#if INTERFACE..#endif" unless
      ** the "-local" command line option is used. */
      return nErr;
    }
    if( (flags & (PS_Interface|PS_Export))==0 ){
      /* typedefs are always local, unless within #if INTERFACE..#endif */
      isLocal = 1;
    }
  }else if( flags & (PS_Static|PS_Local2) ){
    if( proto_static==0 && (flags & PS_Local2)==0 ){
      /* Don't record static variables unless the "-local" command line
      ** option was specified or the "LOCAL" keyword is used. */
      return nErr;
    }
    while( pFirst!=0 && pFirst->pNext!=pEnd &&
       ((pFirst->nText==6 && strncmp(pFirst->zText,"static",6)==0)
        || (pFirst->nText==5 && strncmp(pFirst->zText,"LOCAL",6)==0))
    ){
      /* Lose the initial "static" or local from local variables. 
      ** We'll prepend "extern" later. */
      pFirst = pFirst->pNext;
      isLocal = 1;
    }
    if( pFirst==0 || !isLocal ){
      return nErr;
    }
  }else if( flags & PS_Method ){
    /* Methods are declared by their class.  Don't declare separately. */
    return nErr;
  }
  isVar =  (flags & (PS_Typedef|PS_Method))==0 && isVariableDef(pFirst,pEnd);
  if( isVar && (flags & (PS_Interface|PS_Export|PS_Local))!=0 
  && (flags & PS_Extern)==0 ){
    fprintf(stderr,"%s:%d: Can't define a variable in this context\n",
      zFilename, pFirst->nLine);
    nErr++;
  }
  pName = FindDeclName(pFirst,pEnd->pPrev);
  if( pName==0 ){
    fprintf(stderr,"%s:%d: Can't find a name for the object declared here.\n",
      zFilename, pFirst->nLine);
    return nErr+1;
  }

#ifdef DEBUG
  if( debugMask & PARSER ){
    if( flags & PS_Typedef ){
      printf("**** Found typedef %.*s at line %d...\n",
        pName->nText, pName->zText, pName->nLine);
    }else if( isVar ){
      printf("**** Found variable %.*s at line %d...\n",
        pName->nText, pName->zText, pName->nLine);
    }else{
      printf("**** Found prototype %.*s at line %d...\n",
        pName->nText, pName->zText, pName->nLine);
    }
    PrintTokens(pFirst,pEnd->pPrev);
    printf(";\n");
  }
#endif

  pDecl = CreateDecl(pName->zText,pName->nText);
  if( (flags & PS_Typedef) ){
    DeclSetProperty(pDecl, TY_Typedef);
  }else if( isVar ){
    DeclSetProperty(pDecl,DP_ExternReqd | TY_Variable);
    if( !(flags & DP_Cplusplus) ){
      DeclSetProperty(pDecl,DP_ExternCReqd);
    }
  }else{
    DeclSetProperty(pDecl, TY_Subroutine);
    if( !(flags & DP_Cplusplus) ){
      DeclSetProperty(pDecl,DP_ExternCReqd);
    }
  }
  pDecl->pComment = pFirst->pComment;
  pDecl->zDecl = TokensToString(pFirst,pEnd->pPrev,";\n",0,0);
  if( isLocal || (flags & (PS_Local|PS_Local2))!=0 ){
    DeclSetProperty(pDecl,DP_Local);
  }else if( flags & (PS_Export|PS_Export2) ){
    DeclSetProperty(pDecl,DP_Export);
  }
  if( flags & DP_Cplusplus ){
    DeclSetProperty(pDecl,DP_Cplusplus);
  }
  return nErr;
}