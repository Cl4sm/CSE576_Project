void attachment_delete(void){
  int atn = atoi(PD("atn","0"));
  char *zDocView;
  struct tm *pTm;
  time_t t;
  char **az;
  char zDate[200];

  login_check_credentials();
  throttle(1,1);
  az = db_query("SELECT tn, size, date, user, mime, fname, description "
                "FROM attachment WHERE atn=%d", atn);
  if( az[0]==0 ){
    if( !g.okDelete ){
      common_err("Access denied");
    }else{
      common_err("No such attachment: %d", atn);
    }
  }
  t = atoi(az[2]);
  if( is_user_page(az[0]) ) {
    /* only admin and the user can manipulate a user's home page */
    if( !g.okAdmin && !is_home_page(az[0]) ){
      common_err("Access denied");
    }
  }else if( !ok_to_delete_attachment(t,az[3]) ){
    common_err("Access denied");
  }
  if( is_integer(az[0]) ){
    if( atoi(az[0]) ){
      zDocView = mprintf("tktview?tn=%t",az[0]);
    }else{
      if( !g.okSetup ){
        common_err("Access denied");
      }
      zDocView = "setup_style";
    }
  }else{
    zDocView = mprintf("wiki?p=%t",az[0]);
  }
  if( P("can") ){
    cgi_redirect(zDocView);
    return;
  }
  if( P("ok") ){
    db_execute("DELETE FROM attachment WHERE atn=%d", atn);
    cgi_redirect(zDocView);
    return;
  }
  common_add_action_item(zDocView, "Cancel");
  common_add_help_item("CvstracAttachment");
  pTm = gmtime(&t);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);
  common_header("Delete Attachment?");
  @ <p>Are you sure you want to delete this attachments?</p>
  @ <blockquote>
  @ %h(az[5]) %h(az[1]) bytes added by %h(az[3]) on %h(zDate) UTC.
  if(az[6] && az[6][0]){
    @ <br>
    output_formatted(az[6], NULL);
    @ <br>
  }
  @ </blockquote>
  @
  @ <form method="POST" action="attach_del">
  @ <input type="hidden" name="atn" value="%d(atn)">
  @ &nbsp;&nbsp;&nbsp;&nbsp;
  @ <input type="submit" name="ok" value="Yes, Delete">
  @ &nbsp;&nbsp;&nbsp;&nbsp;
  @ <input type="submit" name="can" value="No, Cancel">
  @ </form>
  common_footer();
}