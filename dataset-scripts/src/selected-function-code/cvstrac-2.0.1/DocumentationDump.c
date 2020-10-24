static void DocumentationDump(void){
  Decl *pDecl;
  static struct {
    int mask;
    char flag;
  } flagSet[] = {
    { TY_Class,       'c' },
    { TY_Enumeration, 'e' },
    { TY_Structure,   's' },
    { TY_Union,       'u' },
    { TY_Variable,    'v' },
    { TY_Subroutine,  'f' },
    { TY_Typedef,     't' },
    { TY_Macro,       'm' },
    { DP_Export,      'x' },
    { DP_Local,       'l' },
    { DP_Cplusplus,   '+' },
  };

  for(pDecl = pDeclFirst; pDecl; pDecl=pDecl->pNext){
    int i;
    int nLabel = 0;
    char *zDecl;
    char zLabel[50];
    for(i=0; i<sizeof(flagSet)/sizeof(flagSet[0]); i++){
      if( DeclHasProperty(pDecl,flagSet[i].mask) ){
        zLabel[nLabel++] = flagSet[i].flag;
      }
    }
    if( nLabel==0 ) continue;
    zLabel[nLabel] = 0;
    InsertExtraDecl(pDecl);
    zDecl = pDecl->zDecl;
    if( zDecl==0 ) zDecl = pDecl->zFwd;
    printf("%s %s %s %d %d %d %d %d %d\n",
       pDecl->zName,
       zLabel,
       pDecl->zFile,
       pDecl->pComment ? (int)pDecl->pComment/sizeof(Token) : 0,
       pDecl->pComment ? pDecl->pComment->nText+1 : 0,
       pDecl->zIf ? strlen(pDecl->zIf)+1 : 0,
       zDecl ? strlen(zDecl) : 0,
       pDecl->pComment ? pDecl->pComment->nLine : 0,
       pDecl->tokenCode.nText ? pDecl->tokenCode.nText+1 : 0
    );
    if( pDecl->pComment ){
      printf("%.*s\n",pDecl->pComment->nText, pDecl->pComment->zText);
    }
    if( pDecl->zIf ){
      printf("%s\n",pDecl->zIf);
    }
    if( zDecl ){
      printf("%s",zDecl);
    }
    if( pDecl->tokenCode.nText ){
      printf("%.*s\n",pDecl->tokenCode.nText, pDecl->tokenCode.zText);
    }
  }
}