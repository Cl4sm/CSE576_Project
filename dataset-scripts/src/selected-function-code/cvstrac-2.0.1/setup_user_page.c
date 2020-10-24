void setup_user_page(void){
  const char *zWPswd, *zOldWPswd;

  /* The user must be at least the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  /*
  ** The "wpw" query parameter is "yes" if the CVSROOT/passwd file is
  ** writable and "no" if not.  
  ** Change it if it has changed.
  */
  zOldWPswd = db_config("write_cvs_passwd","yes");
  zWPswd = P("wpw");
  if( zWPswd && strcmp(zOldWPswd,zWPswd)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('write_cvs_passwd','%q');",
      zWPswd
    );
    zOldWPswd = zWPswd;
    db_config(0,0);
  }

  /*
  ** Import users out of the CVSROOT/passwd file if the user pressed
  ** the Import Users button.  Only setup can do this.
  */
  if( P("import_users") && g.scm.pxUserRead ){
    g.scm.pxUserRead();
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminUserDatabase");
  common_header("Configure User Database Linkage");
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  if( g.scm.pxUserWrite ){
    @ CVSTrac can update the CVSROOT/passwd file with the usernames and
    @ passwords of all CVSTrac users.  Enable or disable this feature
    @ below.</p>
    @ <p>Write User Changes to CVSROOT/passwd?
    cgi_optionmenu(0, "wpw", zOldWPswd, "Yes", "yes", "No", "no", NULL);
    @ <input type="submit" value="Submit">
    @ </p>
  }
  if( g.scm.pxUserRead ){
    @ <p>
    @ Use the following button to automatically create a CVSTrac user ID
    @ for every user currently named in CVSROOT/passwd.  The new users will
    @ be given the same access permissions as user "anonymous" plus check-out
    @ permission and check-in permission if CVS allows the user to write.</p>
    @ <p><input type="submit" name="import_users" value="Import CVS Users"></p>
    @ </form>
    @ </p>
  }
  common_footer();
}