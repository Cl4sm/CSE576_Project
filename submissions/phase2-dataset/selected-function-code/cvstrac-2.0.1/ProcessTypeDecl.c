static int ProcessTypeDecl(Token *pList, int flags, int *pReset){
  Token *pName, *pEnd;
  Decl *pDecl;
  String str;
  int need_to_collapse = 1;
  int type = 0;

  *pReset = 0;
  if( pList==0 || pList->pNext==0 || pList->pNext->eType!=TT_Id ){
    return 0;
  }
  pName = pList->pNext;

  /* Catch the case of "struct Foo;" and skip it. */
  if( pName->pNext && pName->pNext->zText[0]==';' ){
    *pReset = ';';
    return 0;
  }

  for(pEnd=pName->pNext; pEnd && pEnd->eType!=TT_Braces; pEnd=pEnd->pNext){
    switch( pEnd->zText[0] ){
      case '(':
      case '*':
      case '[':
      case '=':
      case ';':
        return 0;
    }
  }
  if( pEnd==0 ){
    return 0;
  }

  /*
  ** At this point, we know we have a type declaration that is bounded
  ** by pList and pEnd and has the name pName.
  */

  /*
  ** If the braces are followed immedately by a semicolon, then we are
  ** dealing a type declaration only.  There is not variable definition
  ** following the type declaration.  So reset...
  */
  if( pEnd->pNext==0 || pEnd->pNext->zText[0]==';' ){
    *pReset = ';';
    need_to_collapse = 0;
  }else{
    need_to_collapse = 1;
  }

  if( proto_static==0 && (flags & (PS_Local|PS_Export|PS_Interface))==0 ){
    /* Ignore these objects unless they are explicitly declared as interface,
    ** or unless the "-local" command line option was specified. */
    *pReset = ';';
    return 0;
  }

#ifdef DEBUG
  if( debugMask & PARSER ){
    printf("**** Found type: %.*s %.*s...\n",
      pList->nText, pList->zText, pName->nText, pName->zText);
    PrintTokens(pList,pEnd);
    printf(";\n");
  }
#endif

  /*
  ** Create a new Decl object for this definition.  Actually, if this
  ** is a C++ class definition, then the Decl object might already exist,
  ** so check first for that case before creating a new one.
  */
  switch( *pList->zText ){
    case 'c':  type = TY_Class;        break;
    case 's':  type = TY_Structure;    break;
    case 'e':  type = TY_Enumeration;  break;
    case 'u':  type = TY_Union;        break;
    default:   /* Can't Happen */      break;
  }
  if( type!=TY_Class ){
    pDecl = 0;
  }else{
    pDecl = FindDecl(pName->zText, pName->nText);
    if( pDecl && (pDecl->flags & type)!=type ) pDecl = 0;
  }
  if( pDecl==0 ){
    pDecl = CreateDecl(pName->zText,pName->nText);
  }
  if( (flags & PS_Static) || !(flags & (PS_Interface|PS_Export)) ){
    DeclSetProperty(pDecl,DP_Local);
  }
  DeclSetProperty(pDecl,type);

  /* The object has a full declaration only if it is contained within
  ** "#if INTERFACE...#endif" or "#if EXPORT_INTERFACE...#endif" or
  ** "#if LOCAL_INTERFACE...#endif".  Otherwise, we only give it a
  ** forward declaration.
  */
  if( flags & (PS_Local | PS_Export | PS_Interface)  ){
    pDecl->zDecl = TokensToString(pList,pEnd,";\n",0,0);
  }else{
    pDecl->zDecl = 0;
  }
  pDecl->pComment = pList->pComment;
  StringInit(&str);
  StringAppend(&str,"typedef ",0);
  StringAppend(&str,pList->zText,pList->nText);
  StringAppend(&str," ",0);
  StringAppend(&str,pName->zText,pName->nText);
  StringAppend(&str," ",0);
  StringAppend(&str,pName->zText,pName->nText);
  StringAppend(&str,";\n",2);
  pDecl->zFwd = StrDup(StringGet(&str),0);
  StringReset(&str);
  StringInit(&str);
  StringAppend(&str,pList->zText,pList->nText);
  StringAppend(&str," ",0);
  StringAppend(&str,pName->zText,pName->nText);
  StringAppend(&str,";\n",2);
  pDecl->zFwdCpp = StrDup(StringGet(&str),0);
  StringReset(&str);
  if( flags & PS_Export ){
    DeclSetProperty(pDecl,DP_Export);
  }else if( flags & PS_Local ){
    DeclSetProperty(pDecl,DP_Local);
  }

  /* Here's something weird.  ANSI-C doesn't allow a forward declaration
  ** of an enumeration.  So we have to build the typedef into the
  ** definition.
  */
  if( pDecl->zDecl && DeclHasProperty(pDecl, TY_Enumeration) ){
    StringInit(&str);
    StringAppend(&str,pDecl->zDecl,0);
    StringAppend(&str,pDecl->zFwd,0);
    SafeFree(pDecl->zDecl);
    SafeFree(pDecl->zFwd);
    pDecl->zFwd = 0;
    pDecl->zDecl = StrDup(StringGet(&str),0);
    StringReset(&str);
  }

  if( pName->pNext->zText[0]==':' ){
    DeclSetProperty(pDecl,DP_Cplusplus);
  }
  if( pName->nText==5 && strncmp(pName->zText,"class",5)==0 ){
    DeclSetProperty(pDecl,DP_Cplusplus);
  }

  /*
  ** Remove all but pList and pName from the input stream.
  */
  if( need_to_collapse ){
    while( pEnd!=pName ){
      Token *pPrev = pEnd->pPrev;
      pPrev->pNext = pEnd->pNext;
      pEnd->pNext->pPrev = pPrev;
      SafeFree(pEnd);
      pEnd = pPrev;
    }
  }
  return 0;
}