void tkttool(void){
  int tn = atoi(PD("tn","0"));
  const char *zTool = P("t");
  char *zAction;
  const char *azSubst[32];
  int n = 0;

  if( tn==0 || zTool==0 ) cgi_redirect("index");

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);

  zAction = db_short_query("SELECT command FROM tool "
                           "WHERE name='%q'", zTool);
  if( zAction==0 || zAction[0]==0 ){
    cgi_redirect(mprintf("tktview?tn=%d",tn));
  }

  common_standard_menu(0, "search?t=1");
  common_add_action_item(mprintf("tktview?tn=%d", tn), "View");
  common_add_action_item(mprintf("tkthistory?tn=%d", tn), "History");
  add_tkt_tools(zTool,tn);

  common_header("#%d: %h", tn, zTool);

  azSubst[n++] = "TN";
  azSubst[n++] = mprintf("%d",tn);
  azSubst[n++] = 0;

  n = execute_tool(zTool,zAction,0,azSubst);
  free(zAction);
  if( n<=0 ){
    cgi_redirect(mprintf("tktview?tn=%d", tn));
  }
  common_footer();
}