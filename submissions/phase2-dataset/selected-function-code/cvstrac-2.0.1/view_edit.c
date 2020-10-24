void view_edit(void){
  int rn;
  const char *zTitle;
  const char *z;
  const char *zOwner;
  char *zClrKey;
  char *zSQL;
  char *zErr = 0;

  login_check_credentials();
  if( !g.okQuery ){
    login_needed();
    return;
  }
  throttle(1,1);
  db_add_functions();
  view_add_functions(0);
  rn = atoi(PD("rn","0"));
  zTitle = P("t");
  zOwner = PD("w",g.zUser);
  z = P("s");
  zSQL = z ? trim_string(z) : 0;
  zClrKey = trim_string(PD("k",""));
  if( rn>0 && P("del2") ){
    db_execute("DELETE FROM reportfmt WHERE rn=%d", rn);
    cgi_redirect("reportlist");
    return;
  }else if( rn>0 && P("del1") ){
    zTitle = db_short_query("SELECT title FROM reportfmt "
                            "WHERE rn=%d", rn);
    if( zTitle==0 ) cgi_redirect("reportlist");

    common_add_action_item(mprintf("rptview?rn=%d",rn), "Cancel");
    common_header("Are You Sure?");
    @ <form action="rptedit" method="POST">
    @ <p>You are about to delete all traces of the report
    @ <strong>%h(zTitle)</strong> from
    @ the database.  This is an irreversible operation.  All records
    @ related to this report will be removed and cannot be recovered.</p>
    @
    @ <input type="hidden" name="rn" value="%d(rn)">
    @ <input type="submit" name="del2" value="Delete The Report">
    @ <input type="submit" name="can" value="Cancel">
    @ </form>
    common_footer();
    return;
  }else if( P("can") ){
    /* user cancelled */
    cgi_redirect("reportlist");
    return;
  }
  if( zTitle && zSQL ){
    if( zSQL[0]==0 ){
      zErr = "Please supply an SQL query statement";
    }else if( (zTitle = trim_string(zTitle))[0]==0 ){
      zErr = "Please supply a title"; 
    }else if( (zErr = verify_sql_statement(zSQL))!=0 ){
      /* empty... zErr non-zero */
    }else{
      /* check query syntax by actually trying the query */
      db_restrict_access(1);
      zErr = db_query_check("%s", zSQL);
      if( zErr ) zErr = mprintf("%s",zErr);
      db_restrict_access(0);
    }
    if( zErr==0 ){
      if( rn>0 ){
        db_execute("UPDATE reportfmt SET title='%q', sqlcode='%q',"
                   " owner='%q', cols='%q' WHERE rn=%d",
           zTitle, zSQL, zOwner, zClrKey, rn);
      }else{
        db_execute("INSERT INTO reportfmt(title,sqlcode,owner,cols) "
           "VALUES('%q','%q','%q','%q')",
           zTitle, zSQL, zOwner, zClrKey);
        z = db_short_query("SELECT max(rn) FROM reportfmt");
        rn = atoi(z);
      }
      cgi_redirect(mprintf("rptview?rn=%d", rn));
      return;
    }
  }else if( rn==0 ){
    zTitle = "";
    zSQL = 
      @ SELECT
      @   CASE WHEN status IN ('new','active') THEN '#f2dcdc'
      @        WHEN status='review' THEN '#e8e8bd'
      @        WHEN status='fixed' THEN '#cfe8bd'
      @        WHEN status='tested' THEN '#bde5d6'
      @        WHEN status='defer' THEN '#cacae5'
      @        ELSE '#c8c8c8' END AS 'bgcolor',
      @   tn AS '#',
      @   type AS 'Type',
      @   status AS 'Status',
      @   sdate(origtime) AS 'Created',
      @   owner AS 'By',
      @   subsystem AS 'Subsys',
      @   sdate(changetime) AS 'Changed',
      @   assignedto AS 'Assigned',
      @   severity AS 'Svr',
      @   priority AS 'Pri',
      @   title AS 'Title'
      @ FROM ticket
    ;
    zClrKey = 
      @ #ffffff Key:
      @ #f2dcdc Active
      @ #e8e8e8 Review
      @ #cfe8bd Fixed
      @ #bde5d6 Tested
      @ #cacae5 Deferred
      @ #c8c8c8 Closed
    ;
  }else{
    char **az = db_query("SELECT title, sqlcode, owner, cols "
                         "FROM reportfmt WHERE rn=%d",rn);
    if( az[0] ){
      zTitle = az[0];
      zSQL = az[1];
      zOwner = az[2];
      zClrKey = az[3];
    }
    if( P("copy") ){
      rn = 0;
      zTitle = mprintf("Copy Of %s", zTitle);
      zOwner = g.zUser;
    }
  }
  if( zOwner==0 ) zOwner = g.zUser;
  common_add_action_item("reportlist", "Cancel");
  if( rn>0 ){
    common_add_action_item( mprintf("rptedit?rn=%d&del1=1",rn), "Delete");
  }
  common_add_help_item("CvstracReport");
  common_header(rn>0 ? "Edit Report Format":"Create New Report Format");
  if( zErr ){
    @ <blockquote><font color="#ff0000"><b>%h(zErr)</b></font></blockquote>
  }
  @ <form action="rptedit" method="POST">
  @ <input type="hidden" name="rn" value="%d(rn)">
  @ <p>Report Title:<br>
  @ <input type="text" name="t" value="%h(zTitle)" size="60"></p>
  @ <p>Enter a complete SQL query statement against the "TICKET" table:<br>
  @ <textarea name="s" cols="70" rows="20">%h(zSQL)</textarea></p>
  if( g.okAdmin ){
    char **azUsers;
    azUsers = db_query("SELECT id FROM user UNION SELECT '' ORDER BY id");
    @ <p>Report owner:
    cgi_v_optionmenu(0, "w", zOwner, (const char**)azUsers);
    @ </p>
  } else {
    @ <input type="hidden" name="w" value="%h(zOwner)">
  }
  @ <p>Enter an optional color key in the following box.  (If blank, no
  @ color key is displayed.)  Each line contains the text for a single
  @ entry in the key.  The first token of each line is the background
  @ color for that line.<br>
  @ <textarea name="k" cols="50" rows="6">%h(zClrKey)</textarea></p>
  if( !g.okAdmin && strcmp(zOwner,g.zUser)!=0 ){
    @ <p>This report format is owned by %h(zOwner).  You are not allowed
    @ to change it.</p>
    @ </form>
    report_format_hints();
    common_footer();
    return;
  }
  @ <input type="submit" value="Apply Changes">
  if( rn>0 ){
    @ <input type="submit" value="Delete This Report" name="del1">
  }
  @ </form>
  report_format_hints();
  common_footer();
}