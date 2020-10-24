static void ChangeIfContext(
  const char *zIf,       /* The desired #if context */
  GenState *pState       /* Current state of the code generator */
){
  if( zIf==0 ){
    if( pState->zIf==0 ) return;
    StringAppend(pState->pStr,"#endif\n",0);
    pState->zIf = 0;
  }else{
    if( pState->zIf ){
      if( strcmp(zIf,pState->zIf)==0 ) return;
      StringAppend(pState->pStr,"#endif\n",0);
      pState->zIf = 0;
    }
    ScanText(zIf, pState);
    if( pState->zIf!=0 ){
      StringAppend(pState->pStr,"#endif\n",0);
    }
    StringAppend(pState->pStr,"#if ",0);
    StringAppend(pState->pStr,zIf,0);
    StringAppend(pState->pStr,"\n",0);
    pState->zIf = zIf;
  }
}