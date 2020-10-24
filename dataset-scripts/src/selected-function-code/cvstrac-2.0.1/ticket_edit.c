void ticket_edit(void){
  static struct {
    char *zColumn;     /* Name of column in the database */
    char *zName;       /* Name of corresponding query parameter */
    int preserveSpace; /* Preserve initial spaces in text */
    int numeric;       /* Field is a numeric value */
    const char *zOld;  /* Current value of this field */
    const char *zNew;  /* Value of the query parameter */
  } aParm[] = {
    { "type",         "y", 0, 0, },  /* 0 */
    { "status",       "s", 0, 0, },  /* 1 */
    { "derivedfrom",  "d", 0, 1, },  /* 2 */
    { "version",      "v", 0, 0, },  /* 3 */
    { "assignedto",   "a", 0, 0, },  /* 4 */
    { "severity",     "e", 0, 1, },  /* 5 */
    { "priority",     "p", 0, 0, },  /* 6 */
    { "subsystem",    "m", 0, 0, },  /* 7 */
    { "owner",        "w", 0, 0, },  /* 8 */
    { "title",        "t", 0, 0, },  /* 9 */
    { "description",  "c", 1, 0, },  /* 10 */
    { "remarks",      "r", 1, 0, },  /* 11 */
    { "contact",      "n", 0, 0, },  /* 12 */
    { "extra1",      "x1", 0, 0, },  /* 13 */
    { "extra2",      "x2", 0, 0, },  /* 14 */
    { "extra3",      "x3", 0, 0, },  /* 15 */
    { "extra4",      "x4", 0, 0, },  /* 16 */
    { "extra5",      "x5", 0, 0, },  /* 17 */
  };
  int tn = 0;
  int rn = 0;
  int nField;
  int i, j;
  int cnt;
  int isPreview;
  char *zSep;
  char **az;
  const char **azUsers = 0;
  char **azChng = 0;
  char **azMs = 0;
  int nExtra;
  const char *azExtra[5];
  char zPage[30];
  char zSQL[2000];
  char *zErrMsg = 0;

  login_check_credentials();
  if( !g.okWrite ){ login_needed(); return; }
  throttle(1,1);
  isPreview = P("pre")!=0;
  sscanf(PD("tn",""), "%d,%d", &tn, &rn);
  if( tn<=0 ){ cgi_redirect("index"); return; }
  bprintf(zPage,sizeof(zPage),"%d",tn);
  history_update(0);

  if( P("del1") && ok_to_delete_ticket(tn) ){
    char *zTitle = db_short_query("SELECT title FROM ticket "
                                  "WHERE tn=%d", tn);
    if( zTitle==0 ) cgi_redirect("index");

    common_add_action_item(mprintf("tktedit?tn=%h",PD("tn","")), "Cancel");
    common_header("Are You Sure?");
    @ <form action="tktedit" method="POST">
    @ <p>You are about to delete all traces of ticket
    output_ticket(tn,0);
    @ &nbsp;<strong>%h(zTitle)</strong> from
    @ the database.  This is an irreversible operation.  All records
    @ related to this ticket will be removed and cannot be recovered.</p>
    @
    @ <input type="hidden" name="tn" value="%h(PD("tn",""))">
    @ <input type="submit" name="del2" value="Delete The Ticket">
    @ <input type="submit" name="can" value="Cancel">
    @ </form>
    common_footer();
    return;
  }
  if( P("del2") && ok_to_delete_ticket(tn) ){
    db_execute(
       "BEGIN;"
       "DELETE FROM ticket WHERE tn=%d;"
       "DELETE FROM tktchng WHERE tn=%d;"
       "DELETE FROM xref WHERE tn=%d;"
       "DELETE FROM attachment WHERE tn=%d;"
       "COMMIT;", tn, tn, tn, tn);
    if( rn>0 ){
      cgi_redirect(mprintf("rptview?rn=%d",rn));
    }else{
      cgi_redirect("index");
    }
    return;
  }

  /* Check to see how many "extra" ticket fields are defined
  */
  nField = sizeof(aParm)/sizeof(aParm[0]);
  azExtra[0] = db_config("extra1_name",0);
  azExtra[1] = db_config("extra2_name",0);
  azExtra[2] = db_config("extra3_name",0);
  azExtra[3] = db_config("extra4_name",0);
  azExtra[4] = db_config("extra5_name",0);
  for(i=nExtra=0; i<5; i++){
    if( azExtra[i]!=0 ){
      nExtra++;
    }else{
      aParm[13+i].zColumn = 0;
    }
  }

  /* Construct a SELECT statement to extract all information we
  ** need from the ticket table.
  */
  j = 0;
  appendf(zSQL,&j,sizeof(zSQL),"SELECT");
  zSep = " ";
  for(i=0; i<nField; i++){
    appendf(zSQL,&j,sizeof(zSQL), "%s%s", zSep,
            aParm[i].zColumn ? aParm[i].zColumn : "''");
    zSep = ",";
  }
  appendf(zSQL,&j,sizeof(zSQL), " FROM ticket WHERE tn=%d", tn);

  /* Execute the SQL.  Load all existing values into aParm[].zOld.
  */
  az = db_query(zSQL);
  if( az==0 || az[0]==0 ){
    cgi_redirect("index");
    return;
  }
  for(i=0; i<nField; i++){
    if( aParm[i].zColumn==0 ) continue;
    aParm[i].zOld = remove_blank_lines(az[i]);
  }

  /* Find out which fields may need to change due to query parameters.
  ** record the new values in aParm[].zNew.
  */
  for(i=cnt=0; i<nField; i++){
    if( aParm[i].zColumn==0 ){ cnt++; continue; }
    aParm[i].zNew = P(aParm[i].zName);
    if( aParm[i].zNew==0 ){
      aParm[i].zNew = aParm[i].zOld;
      if( g.isAnon && aParm[i].zName[0]=='n' ) cnt++;
    }else if( aParm[i].preserveSpace ){
      aParm[i].zNew = remove_blank_lines(aParm[i].zNew);

      /* Only remarks and description fields (i.e. Wiki fields) have
      ** preserve space set. Perfect place to run through edit
      ** heuristics. If it's not allowed, the change won't go through
      ** since the counter won't match.
      */
      zErrMsg = is_edit_allowed(aParm[i].zOld,aParm[i].zNew);
      if( 0==zErrMsg ){
        cnt++;
      }
    }else if( aParm[i].numeric ){
      aParm[i].zNew = extract_integer(aParm[i].zNew);
      cnt++;
    }else{
      aParm[i].zNew = trim_string(aParm[i].zNew);
      cnt++;
    }
  }

  if( g.okCheckout ){
    if( P("cl") && P("ml") ){
      /* The "cl" query parameter holds a list of integer check-in numbers that
      ** this ticket is associated with.  Convert the string into a list of
      ** tokens. We'll filter out non-integers later.
      */
      azChng = tokenize_new_line(P("cl"));
      azMs = tokenize_new_line(P("ml"));
    }else{
      /*
      ** Probably a new form, so get the info from the database.
      */
      azChng = db_query( "SELECT xref.cn FROM xref, chng "
                         "WHERE xref.cn=chng.cn AND "
                         "       chng.milestone=0 AND xref.tn=%d", tn);

      azMs = db_query( "SELECT xref.cn FROM xref, chng "
                       "WHERE xref.cn=chng.cn AND "
                       "       chng.milestone>0 AND xref.tn=%d", tn);
    }
  }

  /* Update the record in the TICKET table.  Also update the XREF table.
  */
  if( cnt==nField && P("submit")!=0 ){
    time_t now;
    char **az;
    int first_change;
    int last_change;
    
    time(&now);
    db_execute("BEGIN");
    az = db_query(
        "SELECT MAX(ROWID)+1 FROM tktchng"
    );
    first_change = atoi(az[0]);
    for(i=cnt=0; i<nField; i++){
      if( aParm[i].zColumn==0 ) continue;
      if( strcmp(aParm[i].zOld,aParm[i].zNew)==0 ) continue;
      db_execute("UPDATE ticket SET %s='%q' WHERE tn=%d",
         aParm[i].zColumn, aParm[i].zNew, tn);
      db_execute("INSERT INTO tktchng(tn,user,chngtime,fieldid,oldval,newval) "
          "VALUES(%d,'%q',%d,'%s','%q','%q')",
          tn, g.zUser, now, aParm[i].zColumn, aParm[i].zOld, aParm[i].zNew);
      cnt++;
    }
    az = db_query(
        "SELECT MAX(ROWID) FROM tktchng"
        );
    last_change = atoi(az[0]);
    if( cnt ){
      db_execute("UPDATE ticket SET changetime=%d WHERE tn=%d", now, tn);
    }

    if( g.okCheckout && P("cl") && P("ml") ){
      db_execute("DELETE FROM xref WHERE tn=%d", tn);

      /*
      ** Insert the values into the cross reference table, but only
      ** once (xref could _really_ use a uniqueness constraint).
      */

      if( azChng!=0 ){
        for(i=0; azChng[i]; i++){
          if( is_integer(azChng[i]) && (i==0 || strcmp(azChng[i],azChng[i-1]))){
            db_execute("INSERT INTO xref(tn,cn) "
                       "SELECT %d,cn FROM chng "
                       "       WHERE cn=%d AND milestone=0", 
                       tn, atoi(azChng[i]));
          }
        }
      }
      if( azMs!=0 ){
        for(i=0; azMs[i]; i++){
          if( is_integer(azMs[i]) && (i==0 || strcmp(azMs[i],azMs[i-1]))){
            db_execute("INSERT INTO xref(tn,cn) "
                       "SELECT %d,cn FROM chng "
                       "       WHERE cn=%d AND milestone>0",
                       tn, atoi(azMs[i]));
          }
        }
      }
    }
    db_execute("COMMIT");
    if( cnt ){
      ticket_notify(tn, first_change, last_change, 0);
    }
    if( rn>0 ){
      cgi_redirect(mprintf("rptview?rn=%d",rn));
    }else{
      cgi_redirect(mprintf("tktview?tn=%d,%d",tn,rn));
    }
    return;
  }

  /* Print the header.
  */
  common_add_action_item( mprintf("tktview?tn=%d,%d", tn, rn), "Cancel");
  if( ok_to_delete_ticket(tn) ){
    common_add_action_item( mprintf("tktedit?tn=%d,%d&del1=1", tn, rn),
                            "Delete");
  }
  common_add_help_item("CvstracTicket");
  common_header("Edit Ticket #%d", tn);

  @ <form action="tktedit" method="POST">
  @ 
  @ <input type="hidden" name="tn" value="%d(tn),%d(rn)">
  @ <nobr>Ticket Number: %d(tn)</nobr><br>
  if( zErrMsg ){
    @ <blockquote>
    @ <font color="red">%h(zErrMsg)</font>
    @ </blockquote>
  }
  @ <nobr>
  @ Title: <input type="text" name="t" value="%h(aParm[9].zNew)"
  @   maxlength=70 size=70>
  @ </nobr><br>
  @ 
  @ Description:
  @ (<small>See <a href="#format_hints">formatting hints</a></small>)<br>
  @ <textarea name="c" rows="8" cols="70" wrap="virtual">
  @ %h(aParm[10].zNew)
  @ </textarea><br>
  if( isPreview ){
    @ <table border=1 cellpadding=15 width="100%%"><tr><td>
    output_formatted(aParm[10].zNew, zPage);
    @ &nbsp;</td></tr></table><br>
  }
  @
  @ Remarks:
  @ (<small>See <a href="#format_hints">formatting hints</a></small>)<br>
  @ <textarea name="r" rows="8" cols="70" wrap="virtual">
  @ %h(aParm[11].zNew)
  @ </textarea><br>
  if( isPreview ){
    @ <table border=1 cellpadding=15 width="100%%"><tr><td>
    output_formatted(aParm[11].zNew, zPage);
    @ &nbsp;</td></tr></table><br>
  }
  @ 
  @ <nobr>
  @ Status:
  cgi_v_optionmenu2(0, "s", aParm[1].zNew, (const char**)db_query(
     "SELECT name, value FROM enums WHERE type='status'"));
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Type: 
  cgi_v_optionmenu2(0, "y", aParm[0].zNew, (const char**)db_query(
     "SELECT name, value FROM enums WHERE type='type'"));
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ 
  @ <nobr>
  @ Severity: 
  cgi_optionmenu(0, "e", aParm[5].zNew,
         "1", "1", "2", "2", "3", "3", "4", "4", "5", "5", NULL);
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Assigned To: 
  azUsers = (const char**)db_query(
              "SELECT id FROM user UNION SELECT '' ORDER BY id");
  cgi_v_optionmenu(0, "a", aParm[4].zNew, azUsers);
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Subsystem:
  cgi_v_optionmenu2(0, "m", aParm[7].zNew, (const char**)db_query(
      "SELECT '','' UNION ALL "
      "SELECT name, value FROM enums WHERE type='subsys'"));
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Version: <input type="text" name="v" value="%h(aParm[3].zNew)" size=10>
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Derived From: <input type="text" name="d" value="%h(aParm[2].zNew)" size=10>
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Priority:
  cgi_optionmenu(0, "p", aParm[6].zNew,
         "1", "1", "2", "2", "3", "3", "4", "4", "5", "5", NULL);
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @ 
  @ <nobr>
  @ Owner: 
  cgi_v_optionmenu(0, "w", aParm[8].zNew, azUsers);
  @ </nobr>
  @ &nbsp;&nbsp;&nbsp;
  @
  if( !g.isAnon ){
    @ <nobr>
    @ Contact: <input type="text" name="n" value="%h(aParm[12].zNew)" size=20>
    @ </nobr>
    @ &nbsp;&nbsp;&nbsp;
    @
  }
  for(i=0; i<5; i++){
    char **az;
    char zX[3];

    if( azExtra[i]==0 ) continue;
    az = db_query("SELECT name, value FROM enums "
                   "WHERE type='extra%d'", i+1);
    bprintf(zX, sizeof(zX), "x%d", i+1);
    @ <nobr>
    @ %h(azExtra[i]):
    if( az && az[0] ){
      cgi_v_optionmenu2(0, zX, aParm[13+i].zNew, (const char **)az);
    }else{
      @ <input type="text" name="%h(zX)" value="%h(aParm[13+i].zNew)" size=20>
    }
    db_query_free(az);
    @ </nobr>
    @ &nbsp;&nbsp;&nbsp;
    @
  }
  if( g.okCheckout ){
    /*
    ** Note that we don't filter the output here. If the user typed in
    ** something bad, they should be able to see it.
    */

    @ <nobr>
    @ Associated Check-ins:
    @ <input type="text" name="cl" size=70 value="\
    if( azChng!=0 ){
      for(i=0; azChng[i]; i++){
        @ %s(i?" ":"")%h(azChng[i])\
      }
    }
    @ ">
    @ </nobr>
    @ &nbsp;&nbsp;&nbsp;
    @ <nobr>
    @ Associated Milestones:
    @ <input type="text" name="ml" size=70 value="\
    if( azMs!=0 ){
      for(i=0; azMs[i]; i++){
        @ %s(i?" ":"")%h(azMs[i])\
      }
    }