void ticket_undo(void){
  int tn = 0;
  const char *zUser;
  time_t tm;
  const char *z;
  char **az;
  int i;

  login_check_credentials();
  if( !g.okWrite ){ login_needed(); return; }
  throttle(1,1);
  tn = atoi(PD("tn","-1"));
  zUser = PD("u","");
  tm = atoi(PD("t","0"));
  if( tn<0 || tm==0 || zUser[0]==0 ){ cgi_redirect("index"); return; }
  if( !ok_to_undo_change(tm, zUser) ){
    goto undo_finished;
  }
  if( P("can") ){
    /* user cancelled */
    goto undo_finished;
  }
  if( P("w")==0 ){
    common_standard_menu(0,0);
    common_add_help_item("CvstracTicket");
    common_add_action_item(mprintf("tkthistory?tn=%d",tn), "Cancel");
    common_header("Undo Change To Ticket?");
    @ <p>If you really want to remove the last edit to ticket #%d(tn)
    @ then click on the "OK" link below.  Otherwise, click on "Cancel".</p>
    @ <form method="POST" action="tktundo">
    @ <input type="hidden" name="tn" value="%d(tn)">
    @ <input type="hidden" name="u" value="%t(zUser)">
    @ <input type="hidden" name="t" value="%d(tm)">
    @ <table cellpadding="30">
    @ <tr><td>
    @ <input type="submit" name="w" value="OK">
    @ </td><td>
    @ <input type="submit" name="can" value="Cancel">
    @ </td></tr>
    @ </table>
    @ </form>
    common_footer();
    return;
  }

  /* Make sure the change we are requested to undo is the vary last
  ** change.
  */
  z = db_short_query("SELECT max(chngtime) FROM tktchng WHERE tn=%d", tn);
  if( z==0 || tm!=atoi(z) ){
    goto undo_finished;
  }

  /* If we get this far, it means the user has confirmed that they
  ** want to undo the last change to the ticket.
  */
  db_execute("BEGIN");
  az = db_query("SELECT fieldid, oldval FROM tktchng "
                "WHERE tn=%d AND user='%q' AND chngtime=%d",
                tn, zUser, tm);
  for(i=0; az[i]; i+=2){
    db_execute("UPDATE ticket SET %s='%q' WHERE tn=%d", az[i], az[i+1], tn);
  }
  db_execute("DELETE FROM tktchng WHERE tn=%d AND user='%q' AND chngtime=%d",
             tn, zUser, tm);
  db_execute("COMMIT");

undo_finished:
  cgi_redirect(mprintf("tkthistory?tn=%d",tn));
}  