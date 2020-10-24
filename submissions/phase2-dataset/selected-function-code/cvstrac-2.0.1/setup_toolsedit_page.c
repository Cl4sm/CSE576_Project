void setup_toolsedit_page(void){
  const char *zTool = PD("t","");
  const char *zObject = PD("o","");
  const char *zCommand = PD("c","");
  const char *zDescription = PD("d","");
  const char *zPerms = PD("p","as");
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
  common_add_action_item("setup_tools", "Cancel");
  common_add_action_item(mprintf("setup_toolsedit?t=%h&del=1",zTool),
                         "Delete");
  common_header("External Tools");

  if( P("can") ){
    cgi_redirect("setup_tools");
    return;
  }else if( P("ok") ){
    /* delete it */
    db_execute("DELETE FROM tool WHERE name='%q';", zTool);
    cgi_redirect("setup_tools");
    return;
  }else if( delete && zTool[0] ){
    @ <p>Are you sure you want to delete tool <b>%h(zTool)</b>?</p>
    @
    @ <form method="POST" action="setup_toolsedit">
    @ <input type="hidden" name="t" value="%h(zTool)">
    @ <input type="submit" name="ok" value="Yes, Delete">
    @ <input type="submit" name="can" value="No, Cancel">
    @ </form>
    common_footer();
    return;
  }

  if( P("u") ){
    if( zTool[0] && zPerms[0] && zObject[0] && zCommand[0] ) {
      /* update database and bounce back to listing page. If the
      ** description is empty, we'll survive (and wing it).
      */
      db_execute("REPLACE INTO tool(name,perms,object,command,description) "
                 "VALUES('%q','%q','%q','%q','%q');",
                 zTool, zPerms, zObject, zCommand, zDescription);
    }

    cgi_redirect("setup_tools");
  }
  
  if( zTool[0] ){
    /* grab values from database, if available
    */
    char **az = db_query("SELECT perms, object, command, description "
                         "FROM tool WHERE name='%q';",
                         zTool);
    if( az && az[0] && az[1] && az[2] && az[3] ){
      zPerms = az[0];
      zObject = az[1];
      zCommand = az[2];
      zDescription = az[3];
    }
  }

  @ <form action="%s(g.zPath)" method="POST">
  @ Tool Name: <input type="text" name="t" value="%h(zTool)" size=12>
  cgi_optionmenu(0,"o",zObject,
                 "File","file",
                 "Wiki","wiki",
                 "Ticket","tkt",
                 "Check-in","chng",
                 "Milestone","ms",
                 "Report", "rpt",
                 "Directory", "dir",
                 NULL);
  @ <br>Required Permissions:
  @ <input type="text" name="p" size=16 value="%h(zPerms)"><br>
  @ <br>Command-line:<br>
  @ <textarea name="c" rows="4" cols="60">%h(zCommand)</textarea><br>
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
  @ <p>External scripts and programs and
  @ improper configuration may result in a compromised server.</p>
  @
  @ <p>Be sure to enclose all text substitutions in single-quotes.
  @ (ex <tt>'%%k'</tt>)  Otherwise, a user could cause arbitrary shell
  @ commands to be run on your system.</p>
  @
  @ <p>Text is stripped of all single-quotes and backslashs before it is
  @ substituted, so if the substitution is itself enclosed in single-quotes,
  @ it will always be treated as a single token by the shell.</p>
  @
  @ <p>Each tool can have a minimum permission set defined. See
  @ <a href="userlist">Users</a> for the full list.</p>
  @ </td></tr></table>
  @
  @ The following substitutions are available to all external tools:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%RP</b></td><td>%s(g.scm.zName) repository root</td></tr>
  @ <tr><td><b>%%P</b></td><td>CVSTrac project name</td></tr>
  @ <tr><td><b>%%B</b></td><td>Server base URL</td></tr>
  @ <tr><td><b>%%U</b></td><td>Current user</td></tr>
  @ <tr><td><b>%%UC</b></td><td>User capabilities</td></tr>
  @ <tr><td><b>%%N</b></td><td>Current epoch time</td></tr>
  @ <tr><td><b>%%T</b></td><td>Name of tool</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ File tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%F</b></td><td>Filename</td></tr>
  @ <tr><td><b>%%V1</b></td><td>First version number</td></tr>
  @ <tr><td><b>%%V2</b></td><td>Second version number (i.e. diff)</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Directory tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%F</b></td><td>Directory pathname</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Ticket tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%TN</b></td><td>Ticket number</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Wiki tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%W</b></td><td>Wiki page name</td></tr>
  @ <tr><td><b>%%T1</b></td><td>First timestamp of wiki page</td></tr>
  @ <tr><td><b>%%T2</b></td><td>Second timestamp of wiki page (i.e. diff)
  @            </td></tr>
  @ <tr><td><b>%%C</b></td><td>Temporary file containing content</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Check-in tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%CN</b></td><td>Check-in number</td></tr>
  @ <tr><td><b>%%C</b></td><td>Temporary file containing message</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Milestone tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%MS</b></td><td>Milestone number</td></tr>
  @ <tr><td><b>%%C</b></td><td>Temporary file containing message</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Report tools have the following substitutions available:
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td><b>%%RN</b></td><td>Report number</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ Additionally, external programs have some or all the following
  @ environment variables defined:<br>
  @ REQUEST_METHOD, GATEWAY_INTERFACE, REQUEST_URI, PATH_INFO,
  @ QUERY_STRING, REMOTE_ADDR, HTTP_USER_AGENT, CONTENT_LENGTH,
  @ HTTP_REFERER, HTTP_HOST, CONTENT_TYPE, HTTP_COOKIE
  @ <br>
  common_footer();
}