int throttle(int exitOnOverload,int needCaptcha){
  const char *zLimit;
  double rLimit;
  const char *zAddr;
  char **az;
  double rLoad;
  double rLastLoad;
  int lastAccess;
  time_t now;
  int overload;
  int captcha = CAPTCHA_LOCKOUT;
  int useCaptcha = atoi(db_config("enable_captcha","0"));
  const char *zCookie = P(captcha_cookie_name());
  const char *zUrl = getenv("REQUEST_URI");

  time(&now);

  if( !g.isAnon ) return 0; /* Throttling does not occur for identified users */
  zLimit = db_config("throttle", 0);
  if( zLimit==0 ) return 0; /* Throttling is turned off */
  rLimit = atof(zLimit);
  if( rLimit<=0.0 ) return 0;  /* Throttling is turned off */

  /* Users with valid captcha cookies are okay.
  */
  if( useCaptcha && zCookie && zCookie[0] && atoi(zCookie) > now ){
    /* update the cookie to a new expiry time
    */
    captcha_set_cookie();
    return 0;
  }

  zAddr = getenv("REMOTE_ADDR");
  if( zAddr==0 ) return 0;  /* No remote IP address provided */

  az = db_query("SELECT load, lastaccess, captcha FROM access_load "
                "WHERE ipaddr='%q'", zAddr);
  if( az[0] && az[1] ){
    rLastLoad = rLoad = atof(az[0]);
    lastAccess = atoi(az[1]);
    if( lastAccess>now ) lastAccess = now;
    rLoad = 1.0 + exp(DECAY_RATE*(lastAccess-now))*rLoad;
    if( rLoad>rLimit && rLoad<rLimit*2.0 ){
      /* If the throttler triggers, make sure it locks out the IP address
      ** for at least 1 hour */
      rLoad = rLimit*2.0;
    }
    if( rLoad>rLimit && rLastLoad>rLimit && az[2] && useCaptcha ){
      /* Once the client blows the limit, repeated hits on anything
      ** other than the captcha page will decrement the captcha
      ** counter, eventually resulting in lockout.
      */
      captcha = atoi(az[2])-1;
    }
  }else{
    rLastLoad = rLoad = 1.0;
    lastAccess = 0;
  }

  db_execute("REPLACE INTO access_load(ipaddr,load,lastaccess,captcha) "
             "VALUES('%q',%g,%d,%d)", zAddr, rLoad, now, captcha);
  overload = rLoad>=rLimit;
  if( useCaptcha && (overload || needCaptcha) ){
    if( captcha <= 0 && exitOnOverload ) {
      /* Captcha lockout count exceeded, client is blocked until the
      ** load drops again.
      */
      lockout();
    }

    if( zUrl==0 ) zUrl = "index";
    cgi_redirect(mprintf("captcha?cnxp=%T", zUrl));
  }else if( overload && exitOnOverload ){
    /* Just block the client */
    lockout();
  }
  return overload;
}