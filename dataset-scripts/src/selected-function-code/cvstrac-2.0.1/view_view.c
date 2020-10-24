void view_view(void){
  int count = 0;
  int rn;
  char **az;
  char *zSql;
  char *zTitle;
  char *zOwner;
  char *zClrKey;
  int tabs;

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  rn = atoi(PD("rn","0"));
  if( rn==0 ){
    cgi_redirect("reportlist");
    return;
  }
  tabs = P("tablist")!=0;
  db_add_functions();
  view_add_functions(tabs);
  az = db_query(
    "SELECT title, sqlcode, owner, cols FROM reportfmt WHERE rn=%d", rn);
  if( az[0]==0 ){
    cgi_redirect("reportlist");
    return;
  }
  zTitle = az[0];
  zSql = az[1];

  if( P("order_by") ){
    /*
    ** If the user wants to do a column sort, wrap the query into a sub
    ** query and then sort the results. This is a whole lot easier than
    ** trying to insert an ORDER BY into the query itself, especially
    ** if the query is already ordered.
    */
    int nField = atoi(P("order_by"));
    if( nField > 0 ){
      const char* zDir = PD("order_dir","");
      zDir = !strcmp("ASC",zDir) ? "ASC" : "DESC";
      zSql = mprintf("SELECT * FROM (%s) ORDER BY %d %s", zSql, nField, zDir);
    }
  }

  zOwner = az[2];
  zClrKey = az[3];
  count = 0;
  if( !tabs ){
    struct GenerateHTML sState;

    db_execute("PRAGMA empty_result_callbacks=ON");
    common_standard_menu("rptview", 0);
    common_add_help_item("CvstracReport");
    common_add_action_item(
      mprintf("rptview?tablist=1&%s", getenv("QUERY_STRING")),
      "Raw Data"
    );
    if( g.okAdmin || (g.okQuery && strcmp(g.zUser,zOwner)==0) ){
      common_add_action_item( mprintf("rptedit?rn=%d",rn), "Edit");
    }
    common_add_action_item( mprintf("rptsql?rn=%d",rn), "SQL");
    common_header("%s", zTitle);
    output_color_key(zClrKey, 1, "border=0 cellpadding=3 cellspacing=0");
    @ <table border=1 cellpadding=2 cellspacing=0>
    db_restrict_access(1);
    sState.rn = rn;
    sState.nCount = 0;
    db_callback_query(generate_html, &sState, "%s", zSql);
    db_restrict_access(0);
    @ </table>
  }else{
    db_restrict_access(1);
    db_callback_query(output_tab_separated, &count, "%s", zSql);
    db_restrict_access(0);
    cgi_set_content_type("text/plain");
  }
  if( !tabs ){
    common_footer();
  }
}