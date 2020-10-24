static int query_authorizer(
  void *NotUsed,
  int type,
  const char *zArg1,
  const char *zArg2,
  const char *zArg3,
  const char *zArg4
){
  if( type==SQLITE_SELECT || type==SQLITE_READ ){
    return SQLITE_OK;
#ifdef SQLITE_FUNCTION
  }else if( type==SQLITE_FUNCTION ){
    return SQLITE_OK;
#endif
  }
#ifdef USE_STRICT_AUTHORIZER
  return SQLITE_DENY;
#else
  return SQLITE_IGNORE;
#endif
}