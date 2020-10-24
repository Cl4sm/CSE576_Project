void throttle_info(void){
  const char *zReset = P("reset");
  const char *zOrderBy;
  char **az;
  int limit;
  int i;

  login_check_credentials();
  if( !g.okSetup ){
    login_needed();
    return;
  }
  zOrderBy = PD("ob","1");
  limit = atoi(PD("limit","50"));
  if( zOrderBy[0]=='1' ){
    zOrderBy = "ORDER BY load DESC";
  }else if( zOrderBy[0]=='2' ){
    zOrderBy = "ORDER BY ipaddr";
  }else{
    zOrderBy = "ORDER BY lastaccess DESC";
  }
  if( zReset ){
    time_t now;
    time(&now);
    db_execute("DELETE FROM access_load WHERE lastaccess<%d", now-86400);
  }
  common_add_nav_item("setup", "Main Setup Menu"); 
  common_add_help_item("CvstracAdminAbuse");
  common_add_action_item("info_throttle?reset=1","Remove Older Entries");
  if( limit>0 ){
    common_add_action_item("info_throttle?limit=-1","View All");
  }else{
    common_add_action_item("info_throttle?limit=50","View Top 50");
  }
  common_header("Throttle Results");
  @ Contents of the ACCESS_LOAD table:
  @ <table border="1" cellspacing="0" cellpadding="2">
  @ <tr>
  @ <th><a href="info_throttle?ob=2">IP Address</a></th>
  @ <th><a href="info_throttle?ob=3">Last Access</a></th>
  @ <th><a href="info_throttle?ob=1">Load</a></th></tr>
  az = db_query("SELECT ipaddr, lastaccess, load FROM access_load %s LIMIT %d",
               zOrderBy, limit);
  for(i=0; az[i]; i+=3){
    struct tm *pTm;
    time_t atime;
    char zTime[200];
    atime = atoi(az[i+1]);
    pTm = localtime(&atime);
    strftime(zTime, sizeof(zTime), "%Y-%m-%d %H:%M:%S", pTm);
    @ <tr><td>&nbsp;&nbsp;%h(az[i])&nbsp;&nbsp;</td>
    @ <td>&nbsp;&nbsp;%h(zTime)&nbsp;&nbsp;</td>
    @ <td>&nbsp;&nbsp;%s(az[i+2])&nbsp;&nbsp;</td></tr>
  }
  @ </table>
  @ <p>
  @ <a href="info_throttle?reset=1">Remove older entries</a>
  common_footer();
}