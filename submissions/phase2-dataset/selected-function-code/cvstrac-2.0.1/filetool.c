void filetool(void){
  const char *zFile = P("f");
  const char *zVers1 = PD("v1","");
  const char *zVers2 = PD("v2","");
  const char *zTool = P("t");
  char *zAction;
  const char *azSubst[32];
  int n = 0;

  if( zFile==0 || zTool==0 ) cgi_redirect("index");

  login_check_credentials();
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);

  zAction = db_short_query("SELECT command FROM tool "
                           "WHERE name='%q'", zTool);
  if( zAction==0 || zAction[0]==0 ) cgi_redirect("index");

  common_standard_menu(0, "search?f=1");
  common_add_action_item(mprintf("rlog?f=%T", zFile), "History");
  add_file_tools(zTool,zFile,zVers1,zVers2);

  common_header("%s for /%h", zTool, zFile);

  @ <a href="rlog?f=%T(zFile)">%h(zFile)</a>
  if( zVers1 ){
    char *zFV = mprintf("fileview?f=%T&v=%T", zFile, zVers1);
    @ <a href="%T(zFV)">%h(zVers1)</a><hr>
  }

  azSubst[n++] = "F";
  azSubst[n++] = quotable_string(zFile);
  azSubst[n++] = "V1";
  azSubst[n++] = quotable_string(zVers1);
  azSubst[n++] = "V2";
  azSubst[n++] = quotable_string(zVers2);
  azSubst[n++] = 0;

  n = execute_tool(zTool,zAction,0,azSubst);
  free(zAction);
  if( n<=0 ){
    cgi_redirect(mprintf("rlog?f=%T",zFile));
  }
  common_footer();
}