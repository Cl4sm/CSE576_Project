static char *GetIfString(void){
  Ifmacro *pIf;
  char *zResult = 0;
  int hasIf = 0;
  String str;

  for(pIf = ifStack; pIf; pIf=pIf->pNext){
    if( pIf->zCondition==0 || *pIf->zCondition==0 ) continue;
    if( !hasIf ){
      hasIf = 1;
      StringInit(&str);
    }else{
      StringAppend(&str," && ",4);
    }
    StringAppend(&str,pIf->zCondition,0);
  }
  if( hasIf ){
    zResult = StrDup(StringGet(&str),0);
    StringReset(&str);
  }else{
    zResult = 0;
  }
  return zResult;
}