void setup_backup_page(void){
  char *zDbName = mprintf("%s.db", g.zName);
  char *zBuName = mprintf("%s.db.bu", g.zName);
  const char *zMsg = 0;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  if( P("bkup") ){
    db_execute("BEGIN");
    zMsg = file_copy(zDbName, zBuName);
    db_execute("COMMIT");
  }else if( P("rstr") ){
    db_execute("BEGIN");
    zMsg = file_copy(zBuName, zDbName);
    db_execute("COMMIT");
  }
 
  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminBackup");
  common_header("Backup The Database");
  if( zMsg ){
    @ <p><font color="#ff0000">%s(zMsg)</font></p>
  }
  @ <p>
  @ Use the buttons below to make a safe (atomic) backup or restore
  @ of the database file.   The original database is in the file
  @ named <b>%h(zDbName)</b> and the backup is in 
  @ <b>%h(zBuName)</b>.
  @ </p>
  @
  @ <p>
  @ It is always safe to do a backup.  The worst that can happen is that
  @ you can overwrite a prior backup.  But a restore can destroy your
  @ database if the backup copy you are restoring from is incorrect.
  @ Use caution when doing a restore.
  @ </p>
  @
  @ <form action="%s(g.zPath)" method="POST">
  @ <p><input type="submit" name="bkup" value="Backup"></p>
  @ <p><input type="submit" name="rstr" value="Restore"></p>
  @ </form>
  common_footer();
}