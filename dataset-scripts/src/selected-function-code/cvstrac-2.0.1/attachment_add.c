void attachment_add(void){
  const char *zPage;
  char *zTitle = NULL;
  char *zErr = 0;
  const char *zBack;
  int mxSize = attachment_max();
  int tn = atoi(PD("tn","0"));

  zPage = P("tn");
  if( zPage==0 ){
    common_err("Invalid or missing \"tn\" query parameter");
  }
  login_check_credentials();
  throttle(1,1);
  if( is_integer(zPage) ){
    if( tn ){
      zBack = mprintf("tktview?tn=%d", tn);
      if( !g.okWrite ){
        cgi_redirect(zBack);
      }
    }else{
      zBack = "setup_style";
      if( !g.okSetup ){
        cgi_redirect("index");
      }
    }
  }else{
    zBack = mprintf("wiki?p=%t", zPage);
    if( is_user_page(zPage)  ){
      /* only admins and the user ccan attach to the users home page */
      if( !g.okAdmin && !is_home_page(zPage) ){
        cgi_redirect(zBack);
      }
    }else if( is_wiki_name(zPage)!=strlen(zPage) ){
      common_err("Invalid wiki page name \"tn=%h\"", zPage);
    }else if( !g.okWiki ){
      cgi_redirect(zBack);
    }
  }
  common_add_help_item("CvstracAttachment");
  common_add_action_item(zBack, "Cancel");
  if( P("can") || mxSize<=0 ){
    cgi_redirect(zBack);
  }
  if( P("ok") ){
    const char *zData = P("f");
    const char *zDescription = PD("d","");
    const char *zName = P("f:filename");
    int size = atoi(PD("f:bytes","0"));
    const char *zType = PD("f:mimetype","text/plain");
    const char *z;
    time_t now = time(0);
    char **az;
    int atn;
    if( zData==0 || zName==0 || zName[0]==0 || size<=0 || zType==0 ){
      common_err("Attachment information is missing from the query content");
    }
    if( size>mxSize ){
      zErr = mprintf("Your attachment is too big.  The maximum allowed size "
               "is %dKB but your attachment was %dKB", mxSize/1024, 
               (size+1023)/1024);
    }else{
      sqlite3 *pDb;
      sqlite3_stmt *pStmt;
      const char *zTail;
      int rc;

      for(z=zName; *z; z++){
        if( (*z=='/' || *z=='\\') && z[1]!=0 ){ zName = &z[1]; }
      }

      /*
      ** In order to insert a blob, we need to drop down to raw SQLite 3
      ** calls.
      */
      pDb = db_open();
      rc = sqlite3_prepare( pDb,
          "INSERT INTO "
          "   attachment(atn,tn,size,date,user,description,mime,fname,content) "
          "VALUES(NULL,?,?,?,?,?,?,?,?);",
          -1, &pStmt, &zTail);
      if( rc!=SQLITE_OK ) {
        db_err( sqlite3_errmsg(pDb), 0,
               "/attach_add: unable to add \"%h\"", zName );
      }
      sqlite3_bind_text(pStmt, 1, zPage, -1, SQLITE_STATIC);
      sqlite3_bind_int(pStmt, 2, size);
      sqlite3_bind_int(pStmt, 3, now);
      sqlite3_bind_text(pStmt, 4, g.zUser, -1, SQLITE_STATIC);
      sqlite3_bind_text(pStmt, 5, zDescription, -1, SQLITE_STATIC);
      sqlite3_bind_text(pStmt, 6, zType, -1, SQLITE_STATIC);
      sqlite3_bind_text(pStmt, 7, zName, -1, SQLITE_STATIC);
      sqlite3_bind_blob(pStmt, 8, zData, size, SQLITE_STATIC);
      rc = sqlite3_step(pStmt);
      if( rc!=SQLITE_DONE ) {
        db_err( sqlite3_errmsg(pDb), 0,
               "/attach_add: unable to add \"%h\"", zName );
      }
      sqlite3_finalize(pStmt);

      az = db_query(
          "SELECT MAX(ROWID) FROM attachment"
          );
      atn = atoi(az[0]);
      if( tn ) ticket_notify(atoi(zPage), 0, 0, atn);
      cgi_redirect(zBack);
    }
  }
  if( is_integer(zPage) ){
    if( tn==0 ){
      /* FIXME: Not sure we need a separate page unless there's an error... */
      common_header("Attachments To Setup");
    }else{
      zTitle = db_short_query("SELECT title FROM ticket WHERE tn=%d", tn);
      if( zTitle==0 ){ common_err("No such ticket: #%d", tn); }
      common_header("Attachments To Ticket #%d", tn);
    }
  }else{
    common_header("Attachments to %h", wiki_expand_name(zPage));
  }
  if( zErr ){
    @ <p><font color="red"><b>Error:</b> %h(zErr)</font></p>
  }
  if( is_integer(zPage) && tn ){
    @ <h2>Ticket #%d(tn): %h(zTitle)</h2>
  }
  if( attachment_html(zPage, "<p>Existing attachments:</p>", "")==0 ){
    @ <p>There are currently no attachments on this document.</p>
  }
  @ <p>To add a new attachment 
  @ select the file to attach below an press the "Add Attachment" button.
  @ Attachments may not be larger than %d(mxSize/1024)KB.</p>
  @ <form method="POST" action="attach_add" enctype="multipart/form-data">
  @ File to attach: <input type="file" name="f"><br>
  @ Description:
  @ (<small>See <a href="#format_hints">formatting hints</a></small>)<br>
  @ <textarea name="d" rows="4" cols="70" wrap="virtual">
  @ </textarea><br>
  @ <input type="submit" name="ok" value="Add Attachment">
  @ <input type="submit" name="can" value="Cancel">
  @ <input type="hidden" name="tn" value="%h(zPage)">
  @ </form>
  @ <hr>
  @ <a name="format_hints">
  @ <h3>Formatting Hints:</h3>
  append_formatting_hints();
  common_footer();
}