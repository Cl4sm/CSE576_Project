static int stack_authorizer(
  void *pTop,
  int type,
  const char *zArg1,
  const char *zArg2,
  const char *zArg3,
  const char *zArg4
){
  int code = SQLITE_OK;
  int nTop;
  for(nTop = (int)pTop; nTop>=0; nTop--){
    int rc = pAuthStack[nTop](NULL,type,zArg1,zArg2,zArg3,zArg4);
    /*
    ** We run through the authorizers one by one until we get the end or find
    ** one which returns an explicit denial or we hit the end. Then we
    ** return the "worst" code. Besides DENY and OK, only IGNORE is expected
    ** but future versions may allow other codes.
    */
    if( rc == SQLITE_DENY ){
      return rc;
    }else if( rc != SQLITE_OK ){
      code = rc;
    }
  }
  return code;
}