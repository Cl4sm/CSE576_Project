void setup_repository_page(void){
  const char *zRoot, *zOldRoot;
  const char *zModule, *zOldModule;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  /*
  ** The "r" query parameter is the name of the CVS repository root
  ** directory.  Change it if it has changed.
  */
  zOldRoot = db_config("cvsroot","");
  zRoot = P("r");
  if( zRoot && strcmp(zOldRoot,zRoot)!=0 ){
    db_execute("REPLACE INTO config(name,value) VALUES('cvsroot','%q');",
      zRoot);
    zOldRoot = zRoot;
    db_config(0,0);
  }

  /*
  ** The "m" query parameter is the name of the module within the
  ** CVS repository that this CVSTrac instance is suppose to track.
  ** Change it if it has changed.
  */
  zOldModule = db_config("module","");
  zModule = P("m");
  if( zModule && strcmp(zOldModule,zModule)!=0 ){
    db_execute("REPLACE INTO config(name,value) VALUES('module','%q');",
      zModule);
    zOldModule = zModule;
    db_config(0,0);
  }

  /*
  ** The "rrh" query parameter is present if the user presses the
  ** "Reread Revision History" button.  This causes the CVSROOT/history
  ** file to be reread.  Do this with caution as it erases any edits
  ** to the history that are currently in the database.  Only the
  ** setup user can do this.
  */
  if( P("rrh") ){
    common_add_action_item("setup_repository", "Cancel");
    common_header("Confirm Reread Of Repository");
    @ <h3">WARNING!</h3>
    @ <p>
    @ If you decide to <b>Reconstruct</b> the change history database all
    @ of your check-ins will be renumbered.  This might break links between
    @ tickets and wiki pages and check-ins.  Any edits you may have made
    @ to check-in messages will be undone as well.</p>
    @
    @ <p> A safer alternative is to select <b>Rescan</b> which will attempt
    @ to preserve existing check-in numbers and check-in message changes.
    @ </p>
    @
    @ <p>In either case, you may want to make a <a href="setup_backup">
    @ backup copy</a> of the database so that you can recover if something
    @ goes wrong.</p>
    @
    @ <form action="%s(g.zPath)" method="POST">
    @ <p>
    @ <input type="submit" name="rrh2" value="Reconstruct">
    @ Reconstruct the check-in database from scratch.
    @ </p>
    @ <p>
    @ <input type="submit" name="rrh3" value="Rescan">
    @ Attempt to reuse existing check-in numbers.
    @ </p>
    @ <p>
    @ <input type="submit" name="cancel" value="Cancel">
    @ Do no do anything.
    @ </p>
    @ </form>
    common_footer();
    return;
  }
  if( P("rrh2") ){
    db_execute(
      "BEGIN;"
      "DELETE FROM chng WHERE not milestone;"
      "DELETE FROM filechng;"
      "DELETE FROM file;"
      "UPDATE config SET value=0 WHERE name='historysize';"
      "COMMIT;"
      "VACUUM;"
    );
    db_config(0,0);
    history_update(0);
  }
  if( P("rrh3") ){
    db_execute(
      "BEGIN;"
      "DELETE FROM filechng WHERE rowid NOT IN ("
         "SELECT min(rowid) FROM filechng "
         "GROUP BY filename, vers||'x'"
      ");"
      "DELETE FROM chng WHERE milestone=0 AND cn NOT IN ("
         "SELECT cn FROM filechng"
      ");"
      "UPDATE config SET value=0 WHERE name='historysize';"
      "COMMIT;"
      "VACUUM;"
    );
    db_config(0,0);
    history_update(1);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminRepository");
  common_header("Configure Repository");
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Enter the full pathname of the root directory of the
  @ %s(g.scm.zName) repository in the space provided below.
  if( g.scm.canFilterModules ){
    @ If you want to restrict this 
    @ server to see only a subset of the files contained in the
    @ %s(g.scm.zName) repository
    @ (for example, if you want to see only one module in a 
    @ repository that contains many unrelated modules) then
    @ enter a pathname prefix for the files you want to see in the
    @ second entry box.
  }
  @ </p>
  @ <p><table>
  @ <tr>
  @   <td align="right">%s(g.scm.zName) repository:</td>
  @   <td><input type="text" name="r" size="40" value="%h(zOldRoot)"></td>
  @ </tr>

  if( g.scm.canFilterModules ){
    @ <tr>
    @   <td align="right">Module prefix:</td>
    @   <td><input type="text" name="m" size="40" value="%h(zOldModule)"></td>
    @ </tr>
  }

  @ </table><br>
  @ <input type="submit" value="Submit">
  @ </p>
  @
  @ <p>
  @ After changing the %s(g.scm.zName) repository above, you will generally
  @ want to press the following button to cause the repository history to be
  @ reread from the new repository.  You can also use this button to
  @ resynchronize the database if a prior read
  @ failed or if you have manually changed it (always a bad idea).
  @ <p><input type="submit" name="rrh" value="Reread Repository"></p>
  @ </form>
  @ </p>
  @ <hr>
  common_footer();
}