void browse_dir(void){
  const char *zName;
  char *zDir;
  char *zBase;
  char **az;
  int i, j;
  int n;
  int nRow;
  const char *zCookieName;
  int nCookieLife;

  login_check_credentials();
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);
  common_standard_menu("dir", "search?f=1");
  /* P("sc") is set only when user explicitly switches to Long/Short view,
  ** via action bar link. In that case we make that users preference
  ** persistent via cookie.
  */
  if( P("sc") ){
    zCookieName = mprintf("%t_browse_url",g.zName);
    nCookieLife = 86400*atoi(db_config("browse_url_cookie_life","90"));
    if( nCookieLife ){
      cgi_set_cookie(zCookieName, "dir", 0, nCookieLife);
    }
  }
  zName = PD("d","");
  if( zName==0 ){
    zName = "";
  }
  common_add_help_item("CvstracBrowse");
  if( zName[0] ){
    common_add_action_item(
      mprintf("timeline?x=1&c=2&dm=1&px=%h",zName),
      "Activity"
    );
  }
  add_dir_tools(0,zName);
  zDir = mprintf("%s", zName);
  zBase = strrchr(zDir, '/');
  if( zBase==0 ){
    zBase = zDir;
    zDir = "";
  }else{
    *zBase = 0;
    zBase++;
  }
  if( zName && zName[0] ){
    /* this looks like navigation, but it's relative to the current page
    */
    common_add_action_item("dir", "Top");
    common_add_action_item(mprintf("dir?d=%T",zDir), "Up");
    common_add_action_item(mprintf("dirview?d=%T&sc=1",zName), "Long");
  }else{
    common_add_action_item("dirview?sc=1","Long");
  }
  az = db_query("SELECT base, isdir FROM file WHERE dir='%q' ORDER BY base",
                 zName);
  for(n=0; az[n*2]; n++){}
  if( zName[0] ) n++;
  nRow = (n+3)/4;
  if( zName[0] ){ zName = mprintf("%s/",zName); }
  common_header("Directory /%h", zName);
  /* @ <h2>Contents of directory /%h(zName)</h2> */
  @ <table width="100%%">
  @ <tr>
  for(i=j=0; i<4; i++){
    @ <td valign="top" width="25%%">
    n = 0;
    if( i==0 && zName[0] ){
      @ <a href="dir?d=%T(zDir)">
      common_icon("backup");
      @ </a>&nbsp;<a href="dir?d=%T(zDir)">..</a><br>
      n++;
    }
    while( n<nRow && az[j] ){
      if( atoi(az[j+1]) ){
        @ <a href="dir?d=%T(zName)%T(az[j])">
        common_icon("dir");
        @ </a>&nbsp;<a href="dir?d=%T(zName)%T(az[j])">%h(az[j])/</a><br>
      }else{
        char *zIcon;
        char *zFilename = mprintf("%s%s", zName, az[j]);
        if(is_file_available(zFilename)){
          zIcon = "file";
        }else{
          zIcon = "del";
        }
        if( zFilename!=0 ) free(zFilename);
        @ <a href="rlog?f=%T(zName)%T(az[j])">
        common_icon(zIcon);
        @ </a>&nbsp;<a href="rlog?f=%T(zName)%T(az[j])">%h(az[j])</a><br>
      }
      n++;
      j += 2;
    }
    @ </td>
  }
  @ </tr></table>
  common_footer();
}