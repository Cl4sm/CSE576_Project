void view_list(void){
  char **az;
  int i;

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  common_standard_menu("reportlist", "search?t=1");
  if( g.okQuery ){
    common_add_action_item("rptnew", "New Report Format");
  }
  common_add_help_item("CvstracReport");
  common_header("Available Report Formats");
  az = db_query("SELECT rn, title, owner FROM reportfmt ORDER BY title");
  @ <p>Choose a report format from the following list:</p>
  @ <ol>
  for(i=0; az[i]; i+=3){
    @ <li><a href="rptview?rn=%t(az[i])"
    @        rel="nofollow">%h(az[i+1])</a>&nbsp;&nbsp;&nbsp;
    if( g.okWrite && az[i+2] && az[i+2][0] ){
      @ (by <i>%h(az[i+2])</i>)
    }
    if( g.okQuery ){
      @ [<a href="rptedit?rn=%t(az[i])&amp;copy=1" rel="nofollow">copy</a>]
    }
    if( g.okAdmin || (g.okQuery && strcmp(g.zUser,az[i+2])==0) ){
      @ [<a href="rptedit?rn=%t(az[i])" rel="nofollow">edit</a>]
    }
    @ [<a href="rptsql?rn=%t(az[i])" rel="nofollow">sql</a>]
    @ </li>
  }
  if( g.okQuery ){
    @ <p><li><a href="rptnew">Create a new report format</a></li></p>
  }
  @ </ol>
  common_footer();
}