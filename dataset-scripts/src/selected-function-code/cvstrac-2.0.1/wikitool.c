void wikitool(void){
  const char *zPage = P("p");
  const char *zTool = P("t");
  time_t t1 = atoi(PD("t1","0"));
  time_t t2 = atoi(PD("t2","0"));
  char *zAction;
  const char *azSubst[32];
  int n = 0;
  char **azPage;
  char *zView;

  if( zPage==0 || zTool==0 ) cgi_redirect("index");

  login_check_credentials();
  if( !g.okRdWiki ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);

  azPage = db_query(
    "SELECT text,invtime FROM wiki WHERE name='%q' AND invtime>=%d LIMIT 1",
    zPage, -(t1 ? t1 : time(0))
  );
  if( azPage[0]==0 ) {
    common_err("Wiki page '%s' didn't return anything", zPage);
  }

  if(P("t1")){
    zView = mprintf("wiki?p=%T&t=%d",zPage,t1);
  }else{
    zView = mprintf("wiki?p=%T",zPage);
  }

  zAction = db_short_query("SELECT command FROM tool WHERE name='%q'", zTool);
  if( zAction==0 || zAction[0]==0 ){
    cgi_redirect(zView);
  }

  common_standard_menu(0, "search?w=1");
  common_add_action_item(zView, "View");
  add_wiki_tools(zTool,zPage,t1,t2);

  common_header("%h: %h", zPage, zTool);

  azSubst[n++] = "W";
  azSubst[n++] = quotable_string(zPage);
  if( P("t1") ){
    azSubst[n++] = "T1";
    azSubst[n++] = mprintf("%d",t1);
  }
  if( P("t2") ){
    azSubst[n++] = "T2";
    azSubst[n++] = mprintf("%d",t2);
  }
  azSubst[n++] = 0;

  n = execute_tool(zTool,zAction,azPage[0],azSubst);
  free(zAction);
  if( n<=0 ){
    cgi_redirect(zView);
  }
  common_footer();
}