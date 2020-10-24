static Token *FindDeclName(Token *pFirst, Token *pLast){
  Token *pName = 0;
  Token *p;
  int c;

  if( pFirst==0 || pLast==0 ){
    return 0;
  }
  pLast = pLast->pNext;
  for(p=pFirst; p && p!=pLast; p=p->pNext){
    if( p->eType==TT_Id ){
      static IdentTable sReserved;
      static int isInit = 0;
      static char *aWords[] = { "char", "class", 
       "const", "double", "enum", "extern", "EXPORT", "ET_PROC", 
       "float", "int", "long",
       "PRIVATE", "PROTECTED", "PUBLIC",
       "register", "static", "struct", "sizeof", "signed", "typedef", 
       "union", "volatile", "virtual", "void", };
  
      if( !isInit ){
        int i;
        for(i=0; i<sizeof(aWords)/sizeof(aWords[0]); i++){
          IdentTableInsert(&sReserved,aWords[i],0);
        }
        isInit = 1;
      }
      if( !IdentTableTest(&sReserved,p->zText,p->nText) ){
        pName = p;
      }
    }else if( p==pFirst ){
      continue;
    }else if( (c=p->zText[0])=='[' && pName ){
      break;
    }else if( c=='(' && p->pNext && p->pNext->eType==TT_Id && pName ){
      break;
    }else if( c==':' && p->zText[1]==':' && pName ){
      break;
    }
  }
  return pName;
}