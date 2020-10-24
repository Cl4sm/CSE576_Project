void common_standard_menu(const char *zOmit, const char *zSrchUrl){
  const char *zLimit;
  if( g.okNewTkt ){
    azLink[nLink++] = "tktnew";
    azLink[nLink++] = "Ticket";
  }
  if( g.okCheckout ){
    azLink[nLink++] = default_browse_url();
    azLink[nLink++] = "Browse";
  }
  if( g.okRead ){
    azLink[nLink++] = "reportlist";
    azLink[nLink++] = "Reports";
  }
  if( g.okRdWiki || g.okRead || g.okCheckout ){
    azLink[nLink++] = "timeline";
    azLink[nLink++] = "Timeline";
  }
  if( g.okRdWiki ){
    azLink[nLink++] = "wiki";
    azLink[nLink++] = "Wiki";
  }
  if( g.okRdWiki || g.okRead || g.okCheckout ){
    azLink[nLink++] = zSrchUrl ? zSrchUrl : "search";
    azLink[nLink++] = "Search";
  }
  if( g.okCheckin ){
    azLink[nLink++] = "msnew";
    azLink[nLink++] = "Milestone";
  }
  if( g.okWrite && !g.isAnon ){
    azLink[nLink++] = "userlist";
    azLink[nLink++] = "Users";
  }
  if( g.okAdmin ){
    azLink[nLink++] = "setup";
    azLink[nLink++] = "Setup";
  }
  azLink[nLink++] = "login";
  if( g.isAnon ){
    azLink[nLink++] = "Login";
  }else{
    azLink[nLink++] = "Logout";
  }
  if( g.isAnon && (zLimit = db_config("throttle",0))!=0 && atof(zLimit)>0.0 ){
    azLink[nLink++] = "honeypot";
    azLink[nLink++] = "0Honeypot";
  }
  if( nLink>2 ){
    azLink[nLink++] = "index";
    azLink[nLink++] = "Home";
  }
  if( zOmit ){
    int j;
    for(j=0; j<nLink; j+=2){
      if( azLink[j][0]==zOmit[0] && strcmp(zOmit,azLink[j])==0 ){
        azLink[j] = azLink[nLink-2];
        azLink[j+1] = azLink[nLink-1];
        nLink -= 2;
        break;
      }
    }
  }
  azLink[nLink] = 0;
}