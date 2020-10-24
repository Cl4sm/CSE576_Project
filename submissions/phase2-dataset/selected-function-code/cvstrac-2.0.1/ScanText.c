static void ScanText(
  const char *zText,    /* The input text to be scanned */
  GenState *pState      /* Current state of the code generator */
){
  int nextValid = 0;    /* True is sNext contains valid data */
  InStream sIn;         /* The input text */
  Token sToken;         /* The current token being examined */
  Token sNext;          /* The next non-space token */

  /* printf("BEGIN SCAN TEXT on %s\n", zText); */

  sIn.z = zText;
  sIn.i = 0;
  sIn.nLine = 1;
  while( sIn.z[sIn.i]!=0 ){
    if( nextValid ){
      sToken = sNext;
      nextValid = 0;
    }else{
      GetNonspaceToken(&sIn,&sToken);
    }
    if( sToken.eType==TT_Id ){
      int needFullDecl;   /* True if we need to provide the full declaration,
                          ** not just the forward declaration */
      Decl *pDecl;        /* The declaration having the name in sToken */

      /*
      ** See if there is a declaration in the database with the name given
      ** by sToken.
      */
      pDecl = FindDecl(sToken.zText,sToken.nText);
      if( pDecl==0 ) continue;

      /* 
      ** If we get this far, we've found an identifier that has a 
      ** declaration in the database.  Now see if we the full declaration
      ** or just a forward declaration.
      */
      GetNonspaceToken(&sIn,&sNext);
      if( sNext.zText[0]=='*' ){
        needFullDecl = 0;
      }else{
        needFullDecl = 1;
        nextValid = sNext.eType==TT_Id;
      }

      /*
      ** Generate the needed declaration.
      */
      DeclareObject(pDecl,pState,needFullDecl);
    }else if( sToken.eType==TT_Preprocessor ){
      sIn.i -= sToken.nText - 1;
    }
  }
  /* printf("END SCANTEXT\n"); */
}