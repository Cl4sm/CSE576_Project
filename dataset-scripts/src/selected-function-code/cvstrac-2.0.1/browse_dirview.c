void browse_dirview(void){
  const char *zName;
  const char *zNameNS; /* NoSlash */
  char *zDir;
  char *zBase;
  char **az;
  int i;
  const char *zCookieName;
  int nCookieLife;
  char *zDesc;
  char *zOrderBy = "1 DESC, 2";
  const char *z;
  char zFld = 0;
  char *zSortUrl = "";

  login_check_credentials();
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);
  common_standard_menu("dirview", "search?f=1");
  /* P("sc") is set only when user explicitly switches to Long/Short view,
  ** via action bar link. In that case we make that users preference
  ** persistent via cookie.
  */
  if( P("sc") ){
    zCookieName = mprintf("%t_browse_url",g.zName);
    nCookieLife = 86400*atoi(db_config("browse_url_cookie_life","90"));
    if( nCookieLife ){
      cgi_set_cookie(zCookieName, "dirview", 0, nCookieLife);
    }
  }
  zName = PD("d","");
  if( zName==0 ){
    zName = "";
  }
  if( zName[0] ){
    common_add_action_item(
      mprintf("timeline?x=1&c=2&dm=1&px=%T",zName),
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

  /* Figure out how should we order this and display our intent in <th>
  ** If no ordering preference is found, don't display anything in <th>
  */
  zDesc = P("desc") ? "DESC" : "ASC";
  
  z = P("o");
  if( z ){
    zSortUrl = mprintf("o=%t%s", z, (zDesc[0]=='D')?"&desc":"");
    zFld = z[0];
    switch( zFld ){
      case 'f':
        zOrderBy = mprintf("2 %s", zDesc);
        break;
      case 'v':
        zOrderBy = mprintf("3 %s", zDesc);
        break;
      case 'u':
        zOrderBy = mprintf("5 %s", zDesc);
        break;
      case 'd':
        zOrderBy = mprintf("6 %s", (strcmp(zDesc,"ASC")==0)?"DESC":"ASC");
        break;
      case 'm':
        zOrderBy = mprintf("7 %s", zDesc);
        break;
      default:
        zFld = 0;
        break;
    }
  }

  if( zName && zName[0] ){
    /* this looks like navigation, but it's relative to the current page
    */
    common_add_action_item(
      mprintf("dirview%s%s",(zSortUrl[0])?"?":"",zSortUrl), "Top");
    common_add_action_item(
      mprintf("dirview?d=%T%s%s",zDir,(zSortUrl[0])?"&":"",zSortUrl), "Up");
    common_add_action_item(mprintf("dir?d=%T&sc=1",zName), "Short");
  }else{
    common_add_action_item("dir?sc=1", "Short");
  }
  common_add_help_item("CvstracBrowse");
  
  zNameNS = mprintf("%s",zName);
  if( zName[0] ){ zName = mprintf("%s/",zName); }
  
  db_add_functions();
  az = db_query(
    "SELECT 0, f.base, fc.vers, fc.chngtype, c.user, c.date, "
    "       '[' || f.lastcn || '] ' || c.message, f.lastcn "
    "FROM file f, chng c, filechng fc "
    "WHERE f.dir='%q' "
    "  AND f.isdir=0 "
    "  AND fc.filename=path(isdir,dir,base) "
    "  AND f.lastcn=fc.cn "
    "  AND f.lastcn=c.cn "
    "UNION ALL "
    "SELECT 1, f.base, NULL, NULL, NULL, c.date, "
    "       NULL, f.lastcn "
    "FROM file f, chng c "
    "WHERE f.dir='%q' "
    "  AND f.isdir=1 "
    "  AND f.lastcn=c.cn "
    "ORDER BY %s",
    zNameNS, zNameNS, zOrderBy
  );
  
  common_header("Directory /%h", zName);
  @ <table width="100%%" border=0 cellspacing=0 cellpadding=3>
  @ <tr>
  column_header(zNameNS,zFld,"file","File");
  column_header(zNameNS,zFld,"vers","Vers");
  column_header(zNameNS,zFld,"user","By");
  column_header(zNameNS,zFld,"date","Age");
  column_header(zNameNS,zFld,"msg","Check-in");
  @ </tr>

  if( zName[0] ){
    @ <tr><td colspan="5">
    @ <a href="dirview?d=%T(zDir)&%s(zSortUrl)">
    common_icon("backup");
    @ </a>&nbsp;<a href="dirview?d=%T(zDir)&%s(zSortUrl)">..</a></td></tr>
  }
  
  /* In case dir is empty, exit nicely */
  if( !az || !az[0] ){
    @ </table>
    common_footer();
    return;
  }
  
  for(i=0; az[i]; i+=8){
    row_content(zName,zSortUrl,i/8,&az[i]);
  }
  @ </table>
  db_query_free(az);
  common_footer();
}