void change_edit(void){
  char *zCancel;
  int cn;
  const char *zMsg, *zOldMsg;
  char **az;

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
  zMsg = P("m");
  if( zMsg ){
    /* Update message in CHNG table and update XREF table in case some 
    ** tickets were removed or added in new message. 
    ** Preserve manually added associations by pre-deleteing only 
    ** those XREF rows that are present in old message.
    */
    db_execute("BEGIN");
    zOldMsg = db_short_query("SELECT message FROM chng WHERE cn=%d", cn);
    db_execute("UPDATE chng SET message='%q' WHERE cn=%d", zMsg, cn);
    xref_remove_checkin_comment(cn, zOldMsg);
    xref_add_checkin_comment(cn, zMsg);
    db_execute("COMMIT");
    cgi_redirect(zCancel);
    return;
  }
  az = db_query("SELECT message, milestone FROM chng WHERE cn=%d", cn);
  zMsg = az[0];
  if( zMsg==0 ) zMsg = "";
  if( az[1][0] && atoi(az[1]) ){
    milestone_edit();
    return;
  }
  common_add_action_item(zCancel, "Cancel");
  common_add_help_item("CvstracCheckin");
  common_header("Edit Check-in [%d]", cn);
  @ <form action="chngedit" method="POST">
  @ <input type="hidden" name="cn" value="%d(cn)">
  @ Edit the change message and press "OK":<br>
  @ <textarea name="m" rows="8" cols="80" wrap="virtual">
  @ %h(zMsg)
  @ </textarea>
  @ <blockquote>
  @ <input type="submit" name="ok" value="OK">
  @ <input type="submit" name="can" value="Cancel">
  @ </blockquote>
  @ </form>
  common_footer();
}