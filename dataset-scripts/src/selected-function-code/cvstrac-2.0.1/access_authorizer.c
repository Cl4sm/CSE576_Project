static int access_authorizer(
  void *NotUsed,
  int type,
  const char *zArg1,
  const char *zArg2,
  const char *zArg3,
  const char *zArg4
){
  if( type==SQLITE_SELECT ){
    return SQLITE_OK;
#ifdef SQLITE_FUNCTION
  }else if( type==SQLITE_FUNCTION ){
    return SQLITE_OK;
#endif
  }else if( type==SQLITE_READ ){
    if( strcasecmp(zArg1,"user")==0 ){
      if( strcasecmp(zArg2,"passwd")==0 || strcasecmp(zArg2,"email")==0 ){
        return SQLITE_IGNORE;
      }
    }else if( strcasecmp(zArg1, "cookie")==0 ){
      return SQLITE_IGNORE;
    }else if( strcasecmp(zArg1, "config")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okSetup && strcasecmp(zArg1, "access_load")==0 ){
      return SQLITE_IGNORE;
    }else if( (!g.okWrite || g.isAnon) && strcasecmp(zArg1,"ticket")==0
        && strcasecmp(zArg2,"contact")==0){
      return SQLITE_IGNORE;
    }else if( !g.okRead && strcasecmp(zArg1,"chng")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okCheckout && strcasecmp(zArg1,"filechng")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okCheckout && strcasecmp(zArg1,"file")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okRead && strcasecmp(zArg1,"inspect")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okRead && strcasecmp(zArg1,"ticket")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okRead && strcasecmp(zArg1,"tktchng")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okRdWiki && strcasecmp(zArg1,"attachment")==0 ){
      return SQLITE_IGNORE;
    }else if( !g.okRdWiki && strcasecmp(zArg1,"wiki")==0 ){
      return SQLITE_IGNORE;
    }
    return SQLITE_OK;
  }else{
    return SQLITE_DENY;
  }
}