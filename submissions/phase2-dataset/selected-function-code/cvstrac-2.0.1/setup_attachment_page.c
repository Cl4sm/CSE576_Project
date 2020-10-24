void setup_attachment_page(void){
  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  if( P("sz") ){
    int sz = atoi(P("sz"))*1024;
    db_execute("REPLACE INTO config VALUES('max_attach_size',%d)", sz);
    db_config(0, 0);
    cgi_redirect("setup");
  }
 
  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminAttachment");
  common_header("Set Maximum Attachment Size");
  @ <p>
  @ Enter the maximum attachment size below.  If you enter a size of
  @ zero, attachments are disallowed.
  @ </p>
  @
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Maximum attachment size in kilobytes: 
  @ <input type="text" name="sz" value="%d(attachment_max()/1024)" size=5>
  @ <input type="submit" value="Set">
  @ </form>
  @ </p>
  common_footer();
}