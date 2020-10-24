void setup_logfile_page(void){
  const char *zLog, *zOldLog;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  /* 
  ** The "log" query parameter specifies a log file into which a record
  ** of all HTTP hits is written.  Write this value if this has changed.
  ** Only setup can make this change.
  */
  zOldLog = db_config("logfile","");
  zLog = P("log");
  if( zLog && strcmp(zOldLog,zLog)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('logfile','%q');",
      zLog
    );
    zOldLog = zLog;
    db_config(0,0);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminLog");
  common_header("Configure Log File");
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Enter the name of file into which is written a log of all accesses
  @ to this server.  Leave the entry blank to disable logging:
  @ </p>
  @ <p>Log File: <input type="text" name="log" size="40" value="%h(zOldLog)">
  @ <input type="submit" value="Submit">
  @ </form>
  @ </p>
  common_footer();
}