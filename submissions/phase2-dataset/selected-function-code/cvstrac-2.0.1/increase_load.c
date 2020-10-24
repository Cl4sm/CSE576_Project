static void increase_load(){
  const char *zAddr = getenv("REMOTE_ADDR");
  const char *zLimit = db_config("throttle", 0);
  double rLimit;

  if( zLimit!=0 && (rLimit = atof(zLimit))>0.0 && zAddr!=0 ){
    time_t now = time(0);
    char *zLastLoad = db_short_query("SELECT load FROM access_load "
                                     "WHERE ipaddr='%q'", zAddr);
    db_execute("REPLACE INTO access_load(ipaddr,load,lastaccess) "
               "VALUES('%q',%g,%d)", zAddr,
               (zLastLoad ? atof(zLastLoad) : 0)
                            + rLimit/WIKI_EDIT_LOCKOUT,
               now);
    if(zLastLoad) free(zLastLoad);
  }

  /* we also need to clear any captcha cookie since having it
  ** bypasses the throttler. This is also handy since it's not
  ** IP specific, so users who are behind variable IP's are going
  ** to still get caught.
  */
  captcha_clear_cookie();
}