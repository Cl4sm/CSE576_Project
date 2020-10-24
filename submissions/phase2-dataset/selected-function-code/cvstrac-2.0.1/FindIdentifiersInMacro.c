static void FindIdentifiersInMacro(Token *pToken, IdentTable *pTable){
  Token sToken;
  InStream sIn;
  int go = 1;

  sIn.z = pToken->zText;
  sIn.i = 1;
  sIn.nLine = 1;
  while( go && sIn.i < pToken->nText ){
    GetToken(&sIn,&sToken);
    switch( sToken.eType ){
      case TT_Id:
        IdentTableInsert(pTable,sToken.zText,sToken.nText);
        break;

      case TT_EOF:
        go = 0;
        break;

      default:
        break;
    }
  }
}