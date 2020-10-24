void login_check_credentials(void){
  const char *zCookie;
  time_t now;
  char **azResult, *z;
  int i;
  const char *zUser;
  const char *zPswd;
  const char *zAddr;     /* The IP address of the browser making this request */
  const char *zAgent;    /* The type of browser */

  g.zUser = g.zHumanName = "anonymous";
  g.okPassword = 0;
  g.okRead = 0;
  g.okNewTkt = 0;
  g.okWrite = 0;
  g.okAdmin = 0;
  g.okSetup = 0;
  g.okCheckout = 0;
  g.okCheckin = 0;
  g.okRdWiki = 0;
  g.okWiki = 0;
  g.okDelete = 0;
  g.okQuery = 0;
  g.isAnon = 1;
  time(&now);

  /*
  ** Check to see if there is an anonymous user.  Everybody gets at
  ** least the permissions that anonymous enjoys.
  */
  z = db_short_query("SELECT capabilities FROM user WHERE id='anonymous'");
  if( z && z[0] ){
    for(i=0; z[i]; i++){
      switch( z[i] ){
        case 'd':   g.okDelete = 1;                  break;
        case 'i':   g.okCheckin = g.okCheckout = 1;  break;
        case 'j':   g.okRdWiki = 1;                  break;
        case 'k':   g.okWiki = g.okRdWiki = 1;       break;
        case 'n':   g.okNewTkt = 1;                  break;
        case 'o':   g.okCheckout = 1;                break;
        case 'p':   g.okPassword = 1;                break;
        case 'q':   g.okQuery = 1;                   break;
        case 'r':   g.okRead = 1;                    break;
        case 'w':   g.okWrite = g.okRead = 1;        break;
      }
    }
  }

  g.okTicketLink = atoi(db_config("anon_ticket_linkinfo","0"));
  g.okCheckinLink = atoi(db_config("anon_checkin_linkinfo","0"));
  g.noFollow = atoi(db_config("nofollow_link","0"));

  /*
  ** Next check to see if the user specified by "U" and "P" query
  ** parameters or by the login cookie exists
  */  
  if( (zUser = P("U"))!=0 && (zPswd = P("P"))!=0 ){
    z = db_short_query("SELECT passwd FROM user WHERE id='%q'", zUser);
    if( z==0 || z[0]==0 || strcmp(crypt(zPswd,z),z)!=0 ){
      return;
    }
  }else if( (zCookie = P(login_cookie_name()))!=0 && zCookie[0]!=0 ){
    zAddr = getenv("REMOTE_ADDR");
    if( zAddr==0 ) zAddr = "0.0.0.0";
    zAgent = getenv("HTTP_USER_AGENT");
    if( zAgent==0 ) zAgent = "Unknown";
    zUser = db_short_query(
      "SELECT user FROM cookie "
      "WHERE cookie='%q' "
      "  AND ipaddr='%q' "
      "  AND agent='%q' "
      "  AND expires>%d",
      zCookie, zAddr, zAgent, now);
    if( zUser==0 ){
      return;
    }
  }else{
    return;
  }

  /* If we reach here, it means that the user named "zUser" checks out.
  ** Set up appropriate permissions.
  */
  azResult = db_query(
    "SELECT name, capabilities FROM user "
    "WHERE id='%q'", zUser
  );
  if( azResult[0]==0 ){
    return;  /* Should never happen... */
  }
  g.isAnon = 0;
  g.zHumanName = azResult[0];
  g.zUser = zUser;
  cgi_logfile(0, g.zUser);
  for(i=0; azResult[1][i]; i++){
    switch( azResult[1][i] ){
      case 's':   g.okSetup = g.okDelete = 1;
      case 'a':   g.okAdmin = g.okRead = g.okWrite = g.okQuery =
                              g.okNewTkt = g.okPassword = 1;
      case 'i':   g.okCheckin = g.okCheckout = 1;  break;
      case 'd':   g.okDelete = 1;                  break;
      case 'j':   g.okRdWiki = 1;                  break;
      case 'k':   g.okWiki = g.okRdWiki = 1;       break;
      case 'n':   g.okNewTkt = 1;                  break;
      case 'o':   g.okCheckout = 1;                break;
      case 'p':   g.okPassword = 1;                break;
      case 'q':   g.okQuery = 1;                   break;
      case 'r':   g.okRead = 1;                    break;
      case 'w':   g.okWrite = g.okRead = 1;        break;
    }
  }
  g.okTicketLink = atoi(db_config("ticket_linkinfo","1"));
  g.okCheckinLink = atoi(db_config("checkin_linkinfo","0"));
}