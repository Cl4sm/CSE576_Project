void dirtool(void){
  const char *zDir = PD("d","");
  const char *zTool = P("t");
  char *zDirUrl;
  char *zAction;
  const char *azSubst[32];
  int n = 0;

  if( zDir==0 || zTool==0 ) cgi_redirect("index");

  login_check_credentials();
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);

  zDirUrl = mprintf("%T?d=%T", default_browse_url(), zDir);

  zAction = db_short_query("SELECT command FROM tool "
                           "WHERE name='%q'", zTool);
  if( zAction==0 || zAction[0]==0 ) cgi_redirect(zDirUrl);

  common_standard_menu(0, "search?f=1");
  common_add_action_item(zDirUrl,"Directory");

  add_dir_tools(zTool,zDir);

  common_header("%s for /%h", zTool, zDir);

  azSubst[n++] = "F";
  azSubst[n++] = quotable_string(zDir);
  azSubst[n++] = 0;

  n = execute_tool(zTool,zAction,0,azSubst);
  free(zAction);
  if( n<=0 ){
    cgi_redirect(zDirUrl);
  }
  common_footer();
}