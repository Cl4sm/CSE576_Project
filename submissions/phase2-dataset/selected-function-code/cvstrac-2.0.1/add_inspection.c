void add_inspection(void){
  char *zCancel;
  int cn;
  const char *zResult;

  login_check_credentials();
  if( !g.okWrite || !g.okCheckin ){
    login_needed();
    return;
  }
  throttle(1,1);
  cn = atoi(PD("cn","0"));
  if( cn<=0 ){
    cgi_redirect("index");
    return;
  }
  zCancel = mprintf("chngview?cn=%d", cn);
  if( P("can") ){
    cgi_redirect(zCancel);
    return;
  }
  zResult = P("r");
  if( zResult && P("ok") ){
    time_t now;
    time(&now);
    db_execute("INSERT INTO inspect(cn,inspecttime,inspector,result) "
       "VALUES(%d,%d,'%q','%q')",
       cn, now, g.zUser, zResult);
    cgi_redirect(zCancel);
    return;
  }
  common_add_help_item("CvstracInspection");
  common_header("Inspection Report");
  @ <form action="inspect" method="POST">
  @ <input type="hidden" name="cn" value="%d(cn)">
  @ Inspection results:
  @ <input type="text" name="r" size="40">
  @ <p><input type="submit" name="ok" value="OK">
  @ <input type="submit" name="can" value="Cancel">
  @ </form>
  common_footer();
}