void embed_view(int rn, const char *zCaption, const char *zTableOpts){
  char **az;
  char *zSql;
  const char *zTitle;
  char *zClrKey;
  static int nDepth = 0;
  struct GenerateHTML sState;

  /* report fields can be wiki formatted. Let's not get into infinite
  ** recursions...
  */
  if(nDepth) return;

  db_add_functions();
  view_add_functions(0);

  az = db_query( "SELECT title, sqlcode, cols FROM reportfmt WHERE rn=%d", rn);
  if( az[0]==0 ) return;
  nDepth++;
  zTitle = az[0];
  if( zCaption==0 || zCaption[0]==0 ){
    zCaption = zTitle;
  }
  zSql = az[1];
  zClrKey = az[3];
  db_execute("PRAGMA empty_result_callbacks=ON");
  /* output_color_key(zClrKey, 1, "border=0 cellpadding=3 cellspacing=0"); */
  @ <div>
  @ <table border=1 cellpadding=2 cellspacing=0
  @        summary="%h(zTitle)" %s(zTableOpts?zTableOpts:"")>
  @ <caption>
  @   <a href="rptview?rn=%d(rn)" rel="nofollow"
  @      title="%h(zTitle)">%h(zCaption)</a>
  @ </caption>
  db_restrict_access(1);
  sState.rn = rn;
  sState.nCount = 0;
  db_callback_query(generate_html, &sState, "%s", zSql);
  db_restrict_access(0);
  @ </table></div>
  nDepth--;
}