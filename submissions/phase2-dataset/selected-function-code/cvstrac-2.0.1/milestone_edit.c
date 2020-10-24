void milestone_edit(void){
  const char *zTime;
  const char *zMsg, *zOldMsg;
  time_t tm;
  int cn;
  int mtype;
  const char *zMType;
  const char *zBr;
  char **az;
  char **azAllBr;
  struct tm *pTm;
  char zDate[200];

  login_check_credentials();
  if( !g.okWrite || !g.okCheckin ){ login_needed(); return; }
  throttle(1,1);
  zTime = P("t");
  if( zTime==0 || (tm = parse_time(zTime))==0 ){
    zTime = "";
  }
  cn = atoi(PD("cn","0"));
  zMsg = remove_blank_lines(PD("m",""));
  zMType = PD("y","0");
  zBr = PD("br","");
  mtype = atoi(zMType);

  if( cn>0 && P("del1") ){
    common_add_action_item(mprintf("msedit?cn=%d",cn), "Cancel");
    common_header("Are You Sure?");
    @ <form action="msedit" method="POST">
    @ <p>You are about to delete all traces of milestone [%d(cn)] from
    @ the database.  This is an irreversible operation.</p>
    @
    @ <input type="hidden" name="cn" value="%d(cn)">
    @ <input type="submit" name="del2" value="Delete The Milestone">
    @ <input type="submit" name="can" value="Cancel">
    @ </form>
    common_footer();
    return;
  }else if( cn>0 && P("del2") ){
    db_execute("BEGIN");
    db_execute("DELETE FROM chng WHERE cn=%d AND milestone", cn);
    db_execute("DELETE FROM xref WHERE cn=%d", cn);
    db_execute("COMMIT");
    cgi_redirect("index");
  }

  if( tm>0 && zTime[0] && zMsg[0] && mtype>0 ){
    if( cn>0 ){
      /*
      ** Preserve manually added associations by pre-deleteing only 
      ** those XREF rows that are present in old message.
      */
      db_execute("BEGIN");
      zOldMsg = db_short_query("SELECT message FROM chng WHERE cn=%d", cn);
      db_execute("UPDATE chng SET date=%d, user='%q', message='%q',"
                 "milestone=%d, branch='%q' "
                 "WHERE cn=%d",
         tm, g.zUser, zMsg, mtype, zBr, cn);
      xref_remove_checkin_comment(cn, zOldMsg);
      xref_add_checkin_comment(cn, zMsg);
      db_execute("COMMIT");
    }else{
      db_execute("BEGIN");
      db_execute("INSERT INTO chng(date,milestone,user,message,branch) "
         "VALUES(%d,%d,'%q','%q','%q')",
         tm, mtype, g.zUser, zMsg, zBr);
      zBr = db_short_query("SELECT max(cn) FROM chng");
      cn = zBr ? atoi(zBr) : 0;
      if( cn>0 ){
        xref_add_checkin_comment(cn, zMsg);
      }
      db_execute("COMMIT");
    }
    cgi_redirect(mprintf("chngview?cn=%d", cn));
    return;
  }
  az = cn<=0 ? 0 :
    db_query("SELECT date, milestone, message, branch FROM chng "
             "WHERE cn=%d AND milestone", cn);
  if( az && az[0] ){
    tm = atoi(az[0]);
    zMType = az[1];
    mtype = atoi(zMType);
    zMsg = az[2];
    zBr = az[3];
  }else{
    time(&tm);
    zMsg = "";
    cn = 0;
    zMType = "1";
    mtype = 1;
    zBr = "";
  }
  azAllBr = db_query("SELECT DISTINCT branch FROM chng WHERE branch NOT NULL "
                     "UNION SELECT '' ORDER BY 1");

  common_add_help_item("CvstracMilestone");
  if( cn>0 ){
    common_standard_menu("msedit", 0);
    common_add_action_item(mprintf("chngview?cn=%d",cn), "Cancel");
    common_add_action_item(mprintf("msedit?cn=%d&del1=1",cn), "Delete");
    common_header("Edit Milestone");
  }else{
    common_standard_menu("msnew", 0);
    common_add_action_item("index", "Cancel");
    common_header("Create New Milestone");
  }
  @ <form action="msedit" method="POST">
  if( cn>0 ){
    @ <input type="hidden" name="cn" value="%d(cn)">
  }
  pTm = localtime(&tm);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);
  @ <table>
  @ <tr><td align="right">Date &amp; Time:</td>
  @    <td><input type="text" name="t" value="%s(zDate)" size=26></td>
  @ <td width=20>&nbsp;</td>
  @ <td>Type: 
  cgi_optionmenu(2,"y", zMType, "Release", "1", "Event", "2", NULL);
  @ </td></tr>
  @ </td></tr>
  @ <tr><td align="right">Branch:</td><td>
  cgi_v_optionmenu(2, "br", zBr, (const char**)azAllBr);
  @ </td></tr>
  @ <tr><td align="right" valign="top">Comment:</td>
  @   <td colspan=3>
  @     <textarea name="m" cols="50" rows="10">%h(zMsg)</textarea></td></tr>
  @ <tr><td colspan=4 align="center">
  @   <input type="submit" value="Submit">
  if( cn>0 ){
    @   &nbsp;&nbsp;&nbsp;
    @   <input type="submit" value="Delete" name="del1">
  }
  @ </td></tr></table>
  @ </form>
  common_footer();
}