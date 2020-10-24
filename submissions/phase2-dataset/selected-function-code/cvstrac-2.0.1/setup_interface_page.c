void setup_interface_page(void){
  int atkt, ack, tkt, ck;
  const char *zBrowseUrl;
  int nCookieLife;

  /* The user must be at least the administrator in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okAdmin ){
    login_needed();
    return;
  }

  if( P("update") ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('anon_ticket_linkinfo','%d');"
      "REPLACE INTO config(name,value) VALUES('anon_checkin_linkinfo','%d');"
      "REPLACE INTO config(name,value) VALUES('ticket_linkinfo','%d');"
      "REPLACE INTO config(name,value) VALUES('checkin_linkinfo','%d');"
      "REPLACE INTO config(name,value) VALUES('browse_url_cookie_life',%d);"
      "REPLACE INTO config(name,value) VALUES('default_browse_url','%q');",
      atoi(PD("atkt","0")),
      atoi(PD("ack","0")),
      atoi(PD("tkt","0")),
      atoi(PD("ck","0")),
      atoi(PD("cl","0")),
      PD("bu","dir")
    );
    db_config(0, 0);
  }

  atkt = atoi(db_config("anon_ticket_linkinfo","0"));
  ack = atoi(db_config("anon_checkin_linkinfo","0"));
  tkt = atoi(db_config("ticket_linkinfo","1"));
  ck = atoi(db_config("checkin_linkinfo","0"));
  zBrowseUrl = db_config("default_browse_url","dir");
  nCookieLife = atoi(db_config("browse_url_cookie_life", "90"));

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminInterface");
  common_header("Configure User Interface");

  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Ticket and check-in/milestone link information enables link tooltips
  @ in most browsers. For example,
  @ <a href="tktview?tn=1" title="First ticket">#1</a> and
  @ <a href="chngview?cn=1" title="Check-in [1]: First check-in
  @   (By anonymous)">[1]</a>. While this provides information to the
  @ user without having to follow a link, it is additional database
  @ load for the server and can increase the size of the web
  @ pages considerably. Check-in link information is usually only useful
  @ if your users put a lot of check-in links within wikis or
  @ remarks.
  @ </p>
  @ <p>
  @ <label for="atkt"><input type="checkbox" name="atkt" id="atkt"
  @   %s(atkt?" checked":"") value="1">
  @ Turn on ticket link information for anonymous users.</label>
  @ <br>
  @ <label for="ack"><input type="checkbox" name="ack" id="ack"
  @   %s(ack?" checked":"") value="1">
  @ Turn on check-in/milestone link information for anonymous users.</label>
  @ <br>
  @ <label for="tkt"><input type="checkbox" name="tkt" id="tkt"
  @   %s(tkt?" checked":"") value="1">
  @ Turn on ticket link information for logged in users.</label>
  @ <br>
  @ <label for="ck"><input type="checkbox" name="ck" id="ck"
  @   %s(ck?" checked":"") value="1">
  @ Turn on check-in/milestone link information for logged in users.</label>
  @ </p>
  @ <p>
  @ <input type="hidden" name="update" value="1">
  @ <input type="submit" value="Set">
  @ </p>
  @ <p>When browsing the repository there are two ways to list files and
  @ directories. The <em>Short</em> view is a compact listing combining
  @ all files and directories into just four columns. The <em>Long</em> view
  @ shows the most recent repository information for each file.</p>
  @ <p><label for="bu0"><input type="radio" name="bu" id="bu0"
  @    %s(strcmp("dirview",zBrowseUrl)==0?" checked":"") value="dirview">
  @ Long view</label><br>
  @ <label for="bu1"><input type="radio" name="bu" id="bu1"
  @   %s(strcmp("dir",zBrowseUrl)==0?" checked":"") value="dir">
  @ Short</label>
  @ <p>
  @ <input type="hidden" name="update" value="1">
  @ <input type="submit" value="Set">
  @ </p>
  @ <p>
  @ Enter number of days browse mode cookie should be kept by users browser.
  @ This cookie keeps track of user's preferred browse mode across user's
  @ multiple visits.<br>
  @ This applies to all users.<br>
  @ Set it to 0 to disable browse mode cookie.
  @ </p>
  @ <p>
  @ Cookie lifetime: 
  @ <input type="text" name="cl" value="%d(nCookieLife)" size=5> days
  @ <p>
  @ <input type="hidden" name="update" value="1">
  @ <input type="submit" value="Set">
  @ </form>
  @ </p>
  common_footer();
}