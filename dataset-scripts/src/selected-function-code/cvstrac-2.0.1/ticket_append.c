void ticket_append(void){
  int tn, rn;
  char zPage[30];
  int doPreview;
  int doSubmit;
  const char *zText;
  const char *zTn;
  char *zErrMsg = 0;
  char *zTktTitle;

  login_check_credentials();
  if( !g.okWrite ){ login_needed(); return; }
  throttle(1,1);
  tn = rn = 0;
  zTn = PD("tn","");
  sscanf(zTn, "%d,%d", &tn, &rn);
  if( tn<=0 ){ cgi_redirect("index"); return; }
  bprintf(zPage,sizeof(zPage),"%d",tn);
  doPreview = P("pre")!=0;
  doSubmit = P("submit")!=0;
  zText = remove_blank_lines(PD("r",""));
  if( doSubmit ){
    zErrMsg = is_edit_allowed(0,zText);
    if( zText[0] && 0==zErrMsg ){
      time_t now;
      struct tm *pTm;
      char zDate[200];
      const char *zOrig;
      char *zNew;
      char *zSpacer = " {linebreak}\n";
      char *zHLine = "\n\n----\n";
      char **az;
      int change;
      zOrig = db_short_query("SELECT remarks FROM ticket WHERE tn=%d", tn);
      zOrig = remove_blank_lines(zOrig);
      time(&now); 
      pTm = localtime(&now);
      strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);
      if( isspace(zText[0]) && isspace(zText[1]) ) zSpacer = "\n\n";
      if( zOrig[0]==0 ) zHLine = "";
      zNew = mprintf("%s_%s by %s:_%s%s",
                     zHLine, zDate, g.zUser, zSpacer, zText);
      db_execute(
        "BEGIN;"
        "UPDATE ticket SET remarks='%q%q', changetime=%d WHERE tn=%d;"
        "INSERT INTO tktchng(tn,user,chngtime,fieldid,oldval,newval) "
           "VALUES(%d,'%q',%d,'remarks','%q','%q%q');"
        "COMMIT;",
        zOrig, zNew, now, tn,
        tn, g.zUser, now, zOrig, zOrig, zNew
      );
      az = db_query(
          "SELECT MAX(ROWID) FROM tktchng"
          );
      change = atoi(az[0]);
      ticket_notify(tn, change, change, 0);
      cgi_redirect(mprintf("tktview?tn=%h",zTn));
    }
  }
  zTktTitle = db_short_query("SELECT title FROM ticket WHERE tn=%d", tn);
  
  common_add_help_item("CvstracTicket");
  common_add_action_item( mprintf("tktview?tn=%h", zTn), "Cancel");
  common_header("Append Remarks To Ticket #%d", tn);

  if( zErrMsg ){
    @ <blockquote>
    @ <font color="red">%h(zErrMsg)</font>
    @ </blockquote>
  }

  @ <form action="tktappend" method="POST">
  @ <input type="hidden" name="tn" value="%h(zTn)">
  @ Append to #%d(tn):
  cgi_href(zTktTitle, 0, 0, 0, 0, 0, "tktview?tn=%d", tn);
  @ &nbsp;
  @ (<small>See <a href="#format_hints">formatting hints</a></small>)<br>
  @ <textarea name="r" rows="8" cols="70" wrap="virtual">%h(zText)</textarea>
  @ <br>
  @ <p align="center">
  @ <input type="submit" name="submit" value="Apply">
  @ &nbsp;&nbsp;&nbsp;
  @ <input type="submit" name="pre" value="Preview">
  @ </p>
  if( doPreview ){
    @ <table border=1 cellpadding=15 width="100%%"><tr><td>
    output_formatted(zText, zPage);
    @ &nbsp;</td></tr></table><br>
  }
  @ 
  @ </form>
  @ <a name="format_hints">
  @ <hr>
  @ <h3>Formatting Hints:</h3>
  append_formatting_hints();
  common_footer();
}