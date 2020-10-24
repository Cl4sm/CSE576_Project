void user_list(void){
  char **azResult;
  int i;

  login_check_credentials();
  if( !g.okWrite && g.isAnon ){
    login_needed();
    return;
  }
  common_standard_menu("userlist", 0);
  common_add_help_item("CvstracAdminUsers");
  common_add_action_item("useredit", "Add User");
  common_header("User List");
  @ <table cellspacing=0 cellpadding=0 border=0>
  @ <tr>
  @   <th align="right"><nobr>User ID</nobr></th>
  @   <th>&nbsp;&nbsp;&nbsp;Permissions&nbsp;&nbsp;&nbsp;</th>
  @   <th><nobr>In Real Life</nobr></th>
  @ </tr>
  azResult = db_query(
    "SELECT id, name, email, capabilities FROM user ORDER BY id");
  for(i=0; azResult[i]; i+= 4){
    @ <tr>
    @ <td align="right">
    if( g.okAdmin ){
      @ <a href="useredit?id=%t(azResult[i])">
    }
    @ <nobr>%h(azResult[i])</nobr>
    if( g.okAdmin ){
      @ </a>
    }
    @ </td>
    @ <td align="center">%s(azResult[i+3])</td>
    if( azResult[i+2] && azResult[i+2][0] ){
      char *zE = azResult[i+2];
      @ <td align="left"><nobr>%h(azResult[i+1])
      @    (<a href="mailto:%h(zE)">%h(zE)</a>)</nobr></td>
    } else {
      @ <td align="left"><nobr>%h(azResult[i+1])</nobr></td>
    }
    @ </tr>
  }
  @ </table>
  @ <p><hr>
  @ <b>Notes:</b>
  @ <ol>
  @ <li><p>The permission flags are as follows:</p>
  @ <table>
  @ <tr><td>a</td><td width="10"></td>
  @     <td>Admin: Create or delete users and ticket report formats</td></tr>
  @ <tr><td>d</td><td></td>
  @     <td>Delete: Erase anonymous wiki, tickets, and attachments</td></tr>
  @ <tr><td>i</td><td></td>
  @     <td>Check-in: Add new code to the %h(g.scm.zName) repository</td></tr>
  @ <tr><td>j</td><td></td><td>Read-Wiki: View wiki pages</td></tr>
  @ <tr><td>k</td><td></td><td>Wiki: Create or modify wiki pages</td></tr>
  @ <tr><td>n</td><td></td><td>New: Create new tickets</td></tr>
  @ <tr><td>o</td><td></td>
  @     <td>Check-out: Read code out of the %h(g.scm.zName) repository</td></tr>
  @ <tr><td>p</td><td></td><td>Password: Change password</td></tr>
  @ <tr><td>q</td><td></td><td>Query: Create or edit report formats</td></tr>
  @ <tr><td>r</td><td></td><td>Read: View tickets and change histories</td></tr>
  @ <tr><td>s</td><td></td><td>Setup: Change CVSTrac options</td></tr>
  @ <tr><td>w</td><td></td><td>Write: Edit tickets</td></tr>
  @ </table>
  @ </p></li>
  @
  @ <li><p>
  @ If a user named "<b>anonymous</b>" exists, then anyone can access
  @ the server without having to log in.  The permissions on the
  @ anonymous user determine the access rights for anyone who is not
  @ logged in.
  @ </p></li>
  @
  if( !strcmp(g.scm.zSCM,"cvs") ){
    @ <li><p>
    @ You must be using CVS version 1.11 or later in order to give users
    @ read-only access to the repository.
    @ With earlier versions of CVS, all users with check-out
    @ privileges also automatically get check-in privileges.
    @ </p></li>
    @
    @ <li><p>
    @ Changing a users ID or password modifies the <b>CVSROOT/passwd</b>,
    @ <b>CVSROOT/readers</b>, and <b>CVSROOT/writers</b> files in the CVS
    @ repository, if those files have write permission turned on.  Users
    @ IDs in <b>CVSROOT/passwd</b> that are unknown to CVSTrac are preserved.
    if( g.okSetup ){
      @ Use the "Import CVS Users" button on the 
      @ <a href="setup_user">user setup</a> page
      @ to import CVS users into CVSTrac.
    }
    @ </p></li>
  }
  @ </ol>
  common_footer();
}