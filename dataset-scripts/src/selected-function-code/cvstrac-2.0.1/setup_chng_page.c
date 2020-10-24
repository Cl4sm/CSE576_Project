void setup_chng_page(void){
  const char *zNotify, *zOldNotify;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  /*
  ** The "notify" query parameter is the name of a program or script that
  ** is run whenever a ticket is created or modified.  Modify the notify
  ** value if it has changed.  Only setup can do this.
  */
  zOldNotify = db_config("notify","");
  zNotify = P("notify");
  if( zNotify && strcmp(zOldNotify,zNotify)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('notify','%q');",
      zNotify
    );
    zOldNotify = zNotify;
    db_config(0,0);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminNotification");
  common_header("Configure Ticket Change Notification");
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Enter a shell command to run whenever a ticket is
  @ created or modified.  The following substitutions are made
  @ on the string before it is passed to /bin/sh:
  @
  @ <table border=1 cellspacing=0 cellpadding=5 align="right" width="45%%">
  @ <tr><td bgcolor="#e0c0c0">
  @ <big><b>Important Security Note</b></big>
  @
  @ <p>Be sure to enclose all text substitutions in single-quotes.
  @ (ex <tt>'%%d'</tt>)  Otherwise, a user could cause arbitrary shell
  @ commands to be run on your system.</p>
  @  
  @ <p>Text is stripped of all single-quotes and backslashs before it is
  @ substituted, so if the substitution is itself enclosed in single-quotes,
  @ it will always be treated as a single token by the shell.</p>
  @
  @ <p>For best security, use only the <b>%%n</b> substitution and have
  @ a Tcl or Perl script extract other fields directly from the database.</p>
  @ </td></tr></table>
  @
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td width="40"><b>%%a</b></td>
  @     <td>UserID of the person the ticket is assigned to</td></tr>
  @ <tr><td><b>%%A</b></td><td>E-mail address of person assigned to</td></tr>
  @ <tr><td><b>%%c</b></td><td>Contact information for the originator</td></tr>
  @ <tr><td><b>%%d</b></td><td>The description, Wiki format</td></tr>
  @ <tr><td><b>%%D</b></td><td>The description, HTML format</td></tr>
  @ <tr><td><b>%%n</b></td><td>The ticket number</td></tr>
  @ <tr><td><b>%%p</b></td><td>The project name</td></tr>
  @ <tr><td><b>%%r</b></td><td>The remarks section, Wiki format</td></tr>
  @ <tr><td><b>%%R</b></td><td>The remarks section, HTML format</td></tr>
  @ <tr><td><b>%%s</b></td><td>The status of the ticket</td></tr>
  @ <tr><td><b>%%t</b></td><td>The title of the ticket</td></tr>
  @ <tr><td><b>%%u</b></td>
  @     <td>UserID of the person who made this change</td></tr>
  @ <tr><td><b>%%w</b></td><td>UserID of the originator of the ticket</td></tr>
  @ <tr><td><b>%%y</b></td><td>Type of ticket</td></tr>
  @ <tr><td><b>%%f</b></td><td>First TKTCHNG rowid of change set; zero if new record</td></tr>
  @ <tr><td><b>%%l</b></td><td>Last TKTCHNG rowid of change set; zero if new record</td></tr>
  @ <tr><td><b>%%h</b></td><td>attacHment number if change is a new attachment; zero otherwise</td></tr>
  @ <tr><td><b>%%1</b></td><td>First user-defined field</td></tr>
  @ <tr><td><b>%%2</b></td><td>Second user-defined field</td></tr>
  @ <tr><td><b>%%3</b></td><td>Third user-defined field</td></tr>
  @ <tr><td><b>%%4</b></td><td>Fourth user-defined field</td></tr>
  @ <tr><td><b>%%5</b></td><td>Fifth user-defined field</td></tr>
  @ <tr><td><b>%%%%</b></td><td>The literal character "<b>%%</b>"</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ <input type="text" name="notify" size="70" value="%h(zOldNotify)">
  @ <input type="submit" value="Submit">
  @ </form>
  @ </p>
  common_footer();
}