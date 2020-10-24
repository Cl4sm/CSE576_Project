void setup_diff_page(void){
  const char *zDiff, *zOldDiff;
  const char *zList, *zOldList;
  const char *zFilter, *zOldFilter;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  /*
  ** The "diff" query parameter is the name of a program or script that
  ** is run whenever a ticket is created or modified.  Modify the filediff
  ** value if it has changed.  Only setup can do this.
  */
  zOldDiff = db_config("filediff","");
  zDiff = P("diff");
  if( zDiff && strcmp(zOldDiff,zDiff)!=0 ){
    if( zDiff[0] ){
      db_execute(
        "REPLACE INTO config(name,value) VALUES('filediff','%q');",
        zDiff
      );
    }else{
      db_execute("DELETE FROM config WHERE name='filediff'");
    }
    zOldDiff = zDiff;
    db_config(0,0);
  }

  /*
  ** The "list" query parameter is the name of a program or script that
  ** is run whenever a ticket is created or modified.  Modify the filelist
  ** value if it has changed.  Only setup can do this.
  */
  zOldList = db_config("filelist","");
  zList = P("list");
  if( zList && strcmp(zOldList,zList)!=0 ){
    if( zList[0] ){
      db_execute(
        "REPLACE INTO config(name,value) VALUES('filelist','%q');",
        zList
      );
    }else{
      db_execute("DELETE FROM config WHERE name='filelist'");
    }
    zOldList = zList;
    db_config(0,0);
  }

  /*
  ** The "filter" query parameter is the name of a program or script that any
  ** files get filtered through for HTML markup.
  */
  zOldFilter = db_config("filefilter","");
  zFilter = P("filter");
  if( zFilter && strcmp(zOldFilter,zFilter)!=0 ){
    if( zFilter[0] ){
      db_execute(
        "REPLACE INTO config(name,value) VALUES('filefilter','%q');",
        zFilter
      );
    }else{
      db_execute("DELETE FROM config WHERE name='filefilter'");
    }
    zOldFilter = zFilter;
    db_config(0,0);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminFilter");
  common_header("Configure Source Code Diff Program");
  @ <form action="%s(g.zPath)" method="POST">
  @ <h2>File Diff</h2>
  @ <p>Enter a shell command to run in order to compute the difference between
  @ two versions of the same file.  The output can be either plain text
  @ or HTML.  If HTML, then the first non-whitespace character of output
  @ should be a "<".  Otherwise the output will be assumed to be plain text.</p>
  @
  @ <table border=1 cellspacing=0 cellpadding=5 align="right" width="33%%">
  @ <tr><td bgcolor="#e0c0c0">
  @ <big><b>Important Security Note</b></big>
  @
  @ <p>Be sure to enclose the substitutions in single-quotes.
  @ (examples: <tt>'%%F'</tt> or <tt>'%%V2'</tt>)
  @ Otherwise, a user who can check in new files
  @ (with unusual names) can cause arbitrary shell
  @ commands to be run on your system.</p>
  @  
  @ <p>CVSTrac will not attempt to diff a file whose name contains a
  @ single-quote or backslash
  @ so if the substitution is itself enclosed in single-quotes, it will always
  @ be treated as a single token by the shell.</p>
  @ </td></tr></table>
  @
  @ <p>The following substitutions are made prior to executing the program:</p>
  @
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td width="40" valign="top"><b>%%F</b></td>
  if( !strcmp(g.scm.zSCM,"cvs") ){
    @     <td>The name of the RCS file to be diffed.  This is a full
    @         pathname including the "<b>,v</b>" suffix.</td>
  }else{
    @     <td>The name of the file to be diffed.</td>
  }
  @ </tr>
  @ <tr><td><b>%%V1</b></td><td>The oldest version to be diffed</td></tr>
  @ <tr><td><b>%%V2</b></td><td>The newest version to be diffed</td></tr>
  @ <tr><td><b>%%RP</b></td><td>Path to repository</td></tr>
  @ <tr><td><b>%%%%</b></td><td>The literal character "<b>%%</b>"</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ <input type="text" name="diff" size="70" value="%h(zOldDiff)">
  @ <input type="submit" value="Submit">
  @
  @ <p>If you leave the above entry blank, the following command is used:</p>
  @
  @ <blockquote><pre>
  if( !strcmp(g.scm.zSCM,"cvs") ){
    @ rcsdiff -q -r'%%V1' -r'%%V2' -u '%%F'
  }else{
    @ svnlook diff -r '%%V2' '%%RP'
  }
  @ </pre></blockquote>
  @ </form>
  @ </p>
  @ <hr>

  @ <form action="%s(g.zPath)" method="POST">
  @ <h2>File List</h2>
  @ <p>Enter below a shell command to run in order to list the content
  @ of a single version of a file <i>as a diff</i> (i.e. for the first
  @ revision of a file).  The output can be either plain text
  @ or HTML.  If HTML, then the first non-whitespace character of output
  @ should be a "<".  Otherwise the output will be assumed to be plain text.</p>
  @
  @ <p>This command is used to show the content 
  @ of files that are newly added to the repository.</p>
  @
  @ <p>The following substitutions are made prior to executing the program:</p>
  @
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td width="40" valign="top"><b>%%F</b></td>
  if( !strcmp(g.scm.zSCM,"cvs") ){
    @     <td>The name of the RCS file to be listed.  This is a full
    @         pathname including the "<b>,v</b>" suffix.</td>
  }else{
    @     <td>The name of the file to be listed.</td>
  }
  @ </tr>
  @ <tr><td><b>%%V</b></td><td>The version to be listed</td></tr>
  @ <tr><td><b>%%RP</b></td><td>Path to repository</td></tr>
  @ <tr><td><b>%%%%</b></td><td>The literal character "<b>%%</b>"</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ <input type="text" name="list" size="70" value="%h(zOldList)">
  @ <input type="submit" value="Submit">
  @
  @ <p>If you leave the above entry blank, the following command is used:</p>
  @
  @ <blockquote><pre>
  if( !strcmp(g.scm.zSCM,"cvs") ){
    @ co -q -p'%%V' '%%F' | diff -c /dev/null -
  }else{
    @ svnlook cat -r '%%V' '%%RP' '%%F'
  }
  @ </pre></blockquote>
  @ </form>
  @ <hr>
  @ </p>

  @ <form action="%s(g.zPath)" method="POST">
  @ <h2>File Filter</h2>
  @ <p>Enter below a shell command to run in order to filter the contents
  @ of a single version of a file.  The filter should expect the file contents
  @ on standard input. The output can be either plain text
  @ or HTML.  If HTML, then the first non-whitespace character of output
  @ should be a "<".  Otherwise the output will be assumed to be plain text.</p>
  @
  @ <p>This command is used to show the content of files</p>
  @
  @ <p>The following substitutions are made prior to executing the program:</p>
  @
  @ <blockquote>
  @ <table cellspacing="5" cellpadding="0">
  @ <tr><td width="40" valign="top"><b>%%F</b></td>
  if( !strcmp(g.scm.zSCM,"cvs") ){
    @     <td>The name of the file to be diffed.  This is a relative
    @         pathname intended for display and content detection purposes.</td>
  }else{
    @     <td>The name of the file to be diffed.</td>
  }
  @ </tr>
  @ <tr><td><b>%%V</b></td><td>The version to be listed</td></tr>
  @ <tr><td><b>%%RP</b></td><td>Path to repository</td></tr>
  @ <tr><td><b>%%%%</b></td><td>The literal character "<b>%%</b>"</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ <input type="text" name="filter" size="70" value="%h(zOldFilter)">
  @ <input type="submit" value="Submit">
  @
  @ <p>If you leave the above entry blank, output will simply be wrapped with
  @ HTML &lt;PRE&gt; tags and encoded as simple HTML.
  @ </form>
  @ </p>
  common_footer();
}