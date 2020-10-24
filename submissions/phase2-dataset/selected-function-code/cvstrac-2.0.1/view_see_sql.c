void view_see_sql(void){
  int rn;
  char *zTitle;
  char *zSQL;
  char *zOwner;
  char *zClrKey;
  char **az;

  login_check_credentials();
  if( !g.okRead ){
    login_needed();
    return;
  }
  throttle(1,0);
  rn = atoi(PD("rn","0"));
  az = db_query("SELECT title, sqlcode, owner, cols "
                "FROM reportfmt WHERE rn=%d",rn);
  common_standard_menu(0, 0);
  common_add_help_item("CvstracReport");
  common_add_action_item( mprintf("rptview?rn=%d",rn), "View");
  common_header("SQL For Report Format Number %d", rn);
  if( az[0]==0 ){
    @ <p>Unknown report number: %d(rn)</p>
    common_footer();
    return;
  }
  zTitle = az[0];
  zSQL = az[1];
  zOwner = az[2];
  zClrKey = az[3];
  @ <table cellpadding=0 cellspacing=0 border=0>
  @ <tr><td valign="top" align="right">Title:</td><td width=15></td>
  @ <td colspan=3>%h(zTitle)</td></tr>
  @ <tr><td valign="top" align="right">Owner:</td><td></td>
  @ <td colspan=3>%h(zOwner)</td></tr>
  @ <tr><td valign="top" align="right">SQL:</td><td></td>
  @ <td valign="top"><pre>
  @ %h(zSQL)
  @ </pre></td>
  @ <td width=15></td><td valign="top">
  output_color_key(zClrKey, 0, "border=0 cellspacing=0 cellpadding=3");
  @ </td>
  @ </tr></table>
  report_format_hints();
  common_footer();
}