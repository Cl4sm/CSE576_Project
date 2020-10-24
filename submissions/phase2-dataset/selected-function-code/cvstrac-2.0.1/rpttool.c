void rpttool(void){
  int rn = atoi(PD("rn","0"));
  const char *zTool = P("t");
  char *zAction;
  const char *azSubst[32];
  int n = 0;

  if( rn==0 || zTool==0 ) cgi_redirect("index");

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);

  zAction = db_short_query("SELECT command FROM tool "
                           "WHERE name='%q' AND object='rpt'", zTool);
  if( zAction==0 || zAction[0]==0 ){
    cgi_redirect(mprintf("rptview?rn=%d",rn));
  }

  common_standard_menu(0, 0);
  common_add_action_item(mprintf("rptview?rn=%d", rn), "View");
  common_add_action_item( mprintf("rptsql?rn=%d",rn), "SQL");
  add_rpt_tools(zTool,rn);

  common_header("%h (%d)", zTool, rn);

  azSubst[n++] = "RN";
  azSubst[n++] = mprintf("%d",rn);
  azSubst[n++] = 0;

  n = execute_tool(zTool,zAction,0,azSubst);
  free(zAction);
  if( n<=0 ){
    cgi_redirect(mprintf("rptview?rn=%d", rn));
  }
  common_footer();
}