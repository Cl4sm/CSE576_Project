void setup_style_page(void){
  const char *zHeader, *zFooter;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  /*
  ** If both "header" and "footer" query parameters are present, then
  ** change the header and footer to the values of those parameters.
  ** Only the setup user can do this.
  */
  if( P("ok") && P("header") && P("footer") ){
    db_execute(
      "REPLACE INTO config VALUES('header','%q');"
      "REPLACE INTO config VALUES('footer','%q');",
       trim_string(P("header")),
       trim_string(P("footer"))
    );
    db_config(0,0);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminStyle");
  if( attachment_max()>0 ){
    common_add_action_item("attach_add?tn=0", "Attach");
  }
  common_add_action_item("setup_style", "Cancel");
  common_header("Configure Style");
  @ <p>
  @ Enter HTML used for the header and footer of every page.
  @ If you leave these entries blank, default headers and/or footers
  @ are used.  If you enter a filename (beginning with a "/" character)
  @ instead of HTML text, then the
  @ file is read at runtime and used for the header or footer.</p>
  @
  @ <p>
  @ You may attach files to this page which can then be referenced from within
  @ your custom header/footer or from other pages. For example, stylesheets,
  @ JavaScript files, logos, icons, etc can all be attached. These attachments may
  @ be referenced directly by filename (i.e. <i>/filename.png</i>)
  @ rather than using <i>attach_get/89/filename.png</i> links.</p>
  @
  @ <p>Substitutions are made within the header and footer text.  These
  @ substitutions are made to the HTML regardless of whether the HTML
  @ is entered directly below or is read from a file.</p>
  @
  @ <blockquote>
  @ <table>
  @ <tr><td width="40"><b>%%N</b></td><td>The name of the project</td></tr>
  @ <tr><td><b>%%T</b></td><td>The title of the current page</td></tr>
  @ <tr><td><b>%%V</b></td><td>The version number of CVSTrac</td></tr>
  @ <tr><td><b>%%B</b></td><td>CVSTrac base URL</td></tr>
  @ <tr><td><b>%%%%</b></td><td>The literal character "<b>%%</b>"</td></tr>
  @ </table>
  @ </blockquote>
  @
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  zHeader = db_config("header","");
  zFooter = db_config("footer","");

  /* user wants to restore the defaults */
  if( P("def") ){
    zHeader = HEADER;
    zFooter = FOOTER;
  }

  @ Header:<br>
  @ <textarea cols=80 rows=8 name="header">%h(zHeader)</textarea><br>
  @ Footer:<br>
  @ <textarea cols=80 rows=8 name="footer">%h(zFooter)</textarea><br>
  @ <input type="submit" name="ok" value="Submit">
  @ <input type="submit" name="def" value="Default">
  @ <input type="submit" name="can" value="Cancel">
  @ </form>
  @ </p>

  attachment_html("0","","");

  common_footer();
}