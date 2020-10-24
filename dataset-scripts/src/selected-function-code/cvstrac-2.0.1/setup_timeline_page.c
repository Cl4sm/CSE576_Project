void setup_timeline_page(void){
  int nCookieLife;
  int nTTL;
  int nRDL;
  
  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }
  
  if( P("cl") || P("ttl") || P("rdl") ){
    if( P("cl") ){
      int nCookieLife = atoi(P("cl"));
      db_execute("REPLACE INTO config VALUES('timeline_cookie_life',%d)", nCookieLife);
    }
    if( P("ttl") ){
      int nTTL = atoi(P("ttl"));
      db_execute("REPLACE INTO config VALUES('rss_ttl',%d)", nTTL);
    }
    if( P("rdl") ){
      int nRDL = atoi(P("rdl"));
      db_execute("REPLACE INTO config VALUES('rss_detail_level',%d)", nRDL);
    }
    db_config(0, 0);
  }
  
  nCookieLife = atoi(db_config("timeline_cookie_life", "90"));
  nTTL = atoi(db_config("rss_ttl", "60"));
  nRDL = atoi(db_config("rss_detail_level", "5"));
  
  common_add_nav_item("setup", "Main Setup Menu");
  common_header("Timeline & RSS Setup");
  @ <form action="%s(g.zPath)" method="POST">
  @ <p>
  @ Enter number of days timeline cookie should be kept by users browser.
  @ This cookie keeps timeline settings persistent across users multiple visits.<br>
  @ This applies to all users.<br>
  @ Set it to 0 to disable timeline cookie.
  @ </p>
  @ <p>
  @ Cookie lifetime: 
  @ <input type="text" name="cl" value="%d(nCookieLife)" size=5> days
  @ <input type="submit" value="Set">
  @ </p>
  @ <hr>
  @ <p>
  @ RSS feed's TTL (Time To Live) tells RSS readers how long a feed should
  @ be cached before refreshing from the source. Because a refresh
  @ downloads the entire page, in order to avoid excessive use of
  @ bandwidth this shouldn't be set too low. Anything lower then 15
  @ is probably not a very good idea, while 30-60 is most common.
  @ </p>
  @ <p>
  @ Time To Live:
  @ <input type="text" name="ttl" value="%d(nTTL)" size=5> minutes
  @ <input type="submit" value="Set">
  @ </p>
  @ <hr>
  @ <p>
  @ RSS feed's detail level determines how much details will be
  @ embedded in feed.<br>
  @ Higher the detail level, higher the bandwidth usage will be.
  @ </p>
  @ <p>
  @ RSS detail level:<br>
  @ <label for="rdl0"><input type="radio" name="rdl" value="0" id="rdl0"
  @ %s(nRDL==0?" checked":"")> Basic</label><br>
  @ <label for="rdl5"><input type="radio" name="rdl" value="5" id="rdl5"
  @ %s(nRDL==5?" checked":"")> Medium</label><br>
  @ <label for="rdl9"><input type="radio" name="rdl" value="9" id="rdl9"
  @ %s(nRDL==9?" checked":"")> High</label><br>
  @ <input type="submit" value="Set">
  @ </p>
  @ </form>
  common_footer();
}