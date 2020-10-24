void chngtool(void){
  int cn = atoi(PD("cn","0"));
  const char *zTool = P("t");
  char *zAction;
  const char *azSubst[32];
  int n = 0;
  char **az;

  if( cn==0 || zTool==0 ) cgi_redirect("index");

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);

  az = db_query("SELECT date, branch, milestone, user, message, directory "
                "FROM chng WHERE cn=%d", cn);
  if( az==0 || az[0]==0 ){
    cgi_redirect(mprintf("chngview?cn=%d",cn));
  }

  zAction = db_short_query("SELECT command FROM tool "
                           "WHERE name='%q' AND object='%s'",
                           zTool, atoi(az[2])?"ms":"chng");
  if( zAction==0 || zAction[0]==0 ){
    cgi_redirect(mprintf("chngview?cn=%d",cn));
  }

  common_standard_menu(0, "search?c=1");
  common_add_action_item(mprintf("chngview?cn=%d", cn), "Back");
  add_chng_tools(zTool,cn,atoi(az[2]));

  common_header("%h: %d", zTool, cn);

  if( atoi(az[2]) ){
    azSubst[n++] = "MS";
  }else{
    azSubst[n++] = "CN";
  }
  azSubst[n++] = mprintf("%d",cn);
  azSubst[n++] = 0;

  n = execute_tool(zTool,zAction,az[4],azSubst);
  free(zAction);
  if( n<=0 ){
    cgi_redirect(mprintf("chngview?cn=%d", cn));
  }
  common_footer();
}