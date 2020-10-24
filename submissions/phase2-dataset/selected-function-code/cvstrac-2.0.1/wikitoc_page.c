void wikitoc_page(void){
  int i;
  char **az;
  const char *zOrderBy = "1";
  const char *zDesc = "";

  login_check_credentials();
  throttle(0,0);
  if( !g.okRdWiki ){ login_needed(); return; }
  if( P("ctime") ){
    zOrderBy = "min(-invtime)";
  }else if( P("mtime") ){
    zOrderBy = "max(-invtime)";
  }
  if( P("desc") ){
    zDesc = " DESC";
  }
  db_add_functions();
  az = db_query(
    "SELECT name, ldate(min(-invtime)), ldate(max(-invtime)) FROM wiki "
    "GROUP BY name ORDER BY %s%s", zOrderBy, zDesc
  );
  common_standard_menu("wiki", "search?w=1");
  common_add_help_item("CvstracWiki");
  common_header("Wiki Table Of Contents");
  @ <table>
  @ <tr>
  @ <th bgcolor="%s(BG3)" class="bkgnd3">
  @ <a href="%h(g.zPath)">Page Name</th><th width="20"></th>
  @ <th bgcolor="%s(BG3)" class="bkgnd3">
  @ <a href="%h(g.zPath)?ctime=1&desc=1">Created</th><th width="20"></th>
  @ <th bgcolor="%s(BG3)" class="bkgnd3">
  @ <a href="%h(g.zPath)?mtime=1&desc=1">Last Modified</a></th>
  @ </tr>
  for(i=0; az[i]; i+=3){
    @ <tr>
    @ <td><a href="wiki?p=%h(az[i])">%h(az[i])</a></td><td></td>
    @ <td>%h(az[i+1])</td><td></td><td>%h(az[i+2])</td>
    @ </tr>
  }
  @ </table>
  common_footer(); 
}