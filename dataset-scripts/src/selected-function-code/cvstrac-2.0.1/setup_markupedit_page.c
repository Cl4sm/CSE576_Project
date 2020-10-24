void setup_markupedit_page(void){
  const char *zMarkup = PD("m","");
  const char *zType = PD("t","0");
  const char *zFormat = PD("f","");
  const char *zDescription = PD("d","");
  int delete = atoi(PD("del","0"));

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_action_item("setup_markup", "Cancel");
  common_add_action_item(mprintf("setup_markupedit?m=%h&del=1",zMarkup),
                         "Delete");
  common_add_help_item("CvstracAdminMarkup");
  common_header("Custom Wiki Markup");

  if( P("can") ){
    cgi_redirect("setup_markup");
    return;
  }else if( P("ok") ){
    /* delete it */
    db_execute("DELETE FROM markup WHERE markup='%q';", zMarkup);
    cgi_redirect("setup_markup");
    return;
  }else if( delete && zMarkup[0] ){
    @ <p>Are you sure you want to delete markup <b>%h(zMarkup)</b>?</p>
    @
    @ <form method="POST" action="setup_markupedit">
    @ <input type="hidden" name="m" value="%h(zMarkup)">
    @ <input type="submit" name="ok" value="Yes, Delete">
    @ <input type="submit" name="can" value="No, Cancel">
    @ </form>
    common_footer();
    return;
  }

  if( P("u") ){
    if( zMarkup[0] && zType[0] && zFormat[0] ) {
      /* update database and bounce back to listing page. If the
      ** description is empty, we'll survive (and wing it).
      */
      db_execute("REPLACE INTO markup(markup,type,formatter,description) "
                 "VALUES('%q',%d,'%q','%q');",
                 zMarkup, atoi(zType), zFormat, zDescription);
    }

    cgi_redirect("setup_markup");
    return;
  }
  
  if( zMarkup[0] ){
    /* grab values from database, if available
    */
    char **az = db_query("SELECT type, formatter, description "
                         "FROM markup WHERE markup='%q';",
                         zMarkup);
    if( az && az[0] && az[1] && az[2] ){
      zType = az[0];
      zFormat = az[1];
      zDescription = az[2];
    }
  }

  @ <form action="%s(g.zPath)" method="POST">
  @ Markup Name: <input type="text" name="m" value="%h(zMarkup)" size=12>
  cgi_optionmenu(0,"t",zType, "Markup","0", "Block","2",
    "Program Markup","1", "Program Block","3", NULL);
  @ <br>Formatter:<br>
  @ <textarea name="f" rows="4" cols="60">%h(zFormat)</textarea><br>
  @ Description:<br>
  @ <textarea name="d" rows="4" cols="60">%h(zDescription)</textarea><br>
  @ <input type="hidden" name="u">
  @ <input type="submit" value="Set">
  @ </form>
  @
  @ <table border=1 cellspacing=0 cellpadding=5 align="right" width="45%%">
  @ <tr><td bgcolor="#e0c0c0">
  @ <big><b>Important Security Note</b></big>
  @
  @ <p>Program formatters execute external scripts and programs and
  @ improper configuration may result in a compromised server.</p>
  @
  @ <p>Be sure to enclose all text substitutions in single-quotes.
  @ (ex <tt>'%%k'</tt>)  Otherwise, a user could cause arbitrary shell
  @ commands to be run on your system.</p>
  @  
  @ <p>Text is stripped of all single-quotes and backslashs before it is
  @ substituted, so if the substitution is itself enclosed in single-quotes,
  @ it will always be treated as a single token by the shell.</p>
  @ </td></tr></table>
  @
  @ The following substitutions are made on the custom markup:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td width="40"><b>%%m</b></td><td>markup name</td></tr>
  @ <tr><td><b>%%k</b></td><td>markup key</td></tr>
  @ <tr><td><b>%%a</b></td><td>markup arguments</td></tr>
  @ <tr><td><b>%%x</b></td><td>markup arguments or, if empty, key</td></tr>
  @ <tr><td><b>%%b</b></td><td>markup block</td></tr>
  @ <tr><td><b>%%r</b></td><td>%s(g.scm.zName) repository root</td></tr>
  @ <tr><td><b>%%n</b></td><td>CVSTrac project name</td></tr>
  @ <tr><td><b>%%u</b></td><td>Current user</td></tr>
  @ <tr><td><b>%%c</b></td><td>User capabilities</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Additionally, external programs have some or all the following
  @ environment variables defined:<br>
  @ REQUEST_METHOD, GATEWAY_INTERFACE, REQUEST_URI, PATH_INFO,
  @ QUERY_STRING, REMOTE_ADDR, HTTP_USER_AGENT, CONTENT_LENGTH,
  @ HTTP_REFERER, HTTP_HOST, CONTENT_TYPE, HTTP_COOKIE
  @ <br>
  @
  @ <h2>Notes</h2>
  @ <ul>
  @   <li>The markup name is the wiki formatting tag. i.e. a markup named
  @   <b>echo</b> would be invoked using <tt>{echo: key args}</tt></li>
  @   <li>Changing the name of an existing markup may break existing
  @   wiki pages</li>
  @   <li>"Markup" markups are simple string substitutions and are handled
  @   directly by CVSTrac</li>
  @   <li>"Block" markups are paired {markup} and {endmarkup} which get
  @   all the text in between as arguments (%a), with no key.</li>
  @   <li>"Program" markups are handled by running external scripts and
  @   programs. These are more flexible, but there are security risks and
  @   too many may slow down page creation. A Program Markup gets the
  @   arguments on the command line while a Program Block also gets the block
  @   from standard input. Both forms should write HTML to standard output</li>
  @   <li>The Description field is used when enumerating the list of available
  @   custom markups using the {markups} tag. This is included in pages
  @   such as <a href="wiki?p=WikiFormatting">WikiFormatting</a> in order to
  @   document server-specific markups.</li>
  @ </ul>

  common_footer();
}