static int ProcessMethodDef(Token *pFirst, Token *pLast, int flags){
  Token *pCode;
  Token *pClass;
  char *zDecl;
  Decl *pDecl;
  String str;
  int type;

  pCode = pLast;
  pLast = pLast->pPrev;
  while( pFirst->zText[0]=='P' ){
    int rc = 1;
    switch( pFirst->nText ){
      case 6:  rc = strncmp(pFirst->zText,"PUBLIC",6); break;
      case 7:  rc = strncmp(pFirst->zText,"PRIVATE",7); break;
      case 9:  rc = strncmp(pFirst->zText,"PROTECTED",9); break;
      default:  break;
    }
    if( rc ) break;
    pFirst = pFirst->pNext;
  }
  pClass = FindDeclName(pFirst,pLast);
  if( pClass==0 ){
    fprintf(stderr,"%s:%d: Unable to find the class name for this method\n",
       zFilename, pFirst->nLine);
    return 1;
  }
  pDecl = FindDecl(pClass->zText, pClass->nText);
  if( pDecl==0 || (pDecl->flags & TY_Class)!=TY_Class ){
    pDecl = CreateDecl(pClass->zText, pClass->nText);
    DeclSetProperty(pDecl, TY_Class);
  }
  StringInit(&str);
  if( pDecl->zExtra ){
    StringAppend(&str, pDecl->zExtra, 0);
    SafeFree(pDecl->zExtra);
    pDecl->zExtra = 0;
  }
  type = flags & PS_PPP;
  if( pDecl->extraType!=type ){
    if( type & PS_Public ){
      StringAppend(&str, "public:\n", 0);
      pDecl->extraType = PS_Public;
    }else if( type & PS_Protected ){
      StringAppend(&str, "protected:\n", 0);
      pDecl->extraType = PS_Protected;
    }else if( type & PS_Private ){
      StringAppend(&str, "private:\n", 0);
      pDecl->extraType = PS_Private;
    }
  }
  StringAppend(&str, "  ", 0);
  zDecl = TokensToString(pFirst, pLast, ";\n", pClass, 2);
  StringAppend(&str, zDecl, 0);
  SafeFree(zDecl);
  pDecl->zExtra = StrDup(StringGet(&str), 0);
  StringReset(&str);
  return 0;
}