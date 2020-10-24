void ticket_new(void){
  const char *zTitle = trim_string(PD("t",""));
  const char *zType = P("y");
  const char *zVers = PD("v","");
  const char *zDesc = remove_blank_lines(PD("d",""));
  const char *zContact = PD("c","");
  const char *zWho = P("w");
  const char *zSubsys = PD("s","");
  const char *zSev = PD("r",db_config("dflt_severity","1"));
  const char *zPri = PD("p",db_config("dflt_priority","1"));
  const char *zFrom = P("f");
  int isPreview = P("preview")!=0;
  int severity, priority;
  char **az;
  char *zErrMsg = 0;
  int i;

  login_check_credentials();
  if( !g.okNewTkt ){
    login_needed();
    return;
  }
  throttle(1,1);
  severity = atoi(zSev);
  priority = atoi(zPri);

  /* user can enter #tn or just tn, and empty is okay too */
  zFrom = extract_integer(zFrom);

  if( zType==0 ){
    zType = db_config("dflt_tkt_type","code");
  }
  if( zWho==0 ){
    zWho = db_config("assignto","");
  }
  if( zTitle && strlen(zTitle)>70 ){
    zErrMsg = "Please make the title no more than 70 characters long.";
  }
  if( zErrMsg==0 && zTitle[0] && zType[0] && zDesc[0] && P("submit")
      && (zContact[0] || !g.isAnon) ){
    int tn;
    time_t now;
    const char *zState;

    db_execute("BEGIN");
    az = db_query("SELECT max(tn)+1 FROM ticket");
    tn = atoi(az[0]);
    if( tn<=0 ) tn = 1;
    time(&now);
    zState = db_config("initial_state", "new");
    db_execute(
       "INSERT INTO ticket(tn, type, status, origtime,  changetime, "
       "                   version, assignedto, severity, priority, derivedfrom, "
       "                   subsystem, owner, title, description, contact) "
       "VALUES(%d,'%q','%q',%d,%d,'%q','%q',%d,%d,'%q','%q','%q','%q','%q','%q')",
       tn, zType, zState, now, now, zVers, zWho, severity, priority, zFrom, zSubsys,
       g.zUser, zTitle, zDesc, zContact
    );
    for(i=1; i<=5; i++){
      const char *zVal;
      char zX[3];
      bprintf(zX,sizeof(zX),"x%d",i);
      zVal = P(zX);
      if( zVal && zVal[0] ){
        db_execute("UPDATE ticket SET extra%d='%q' WHERE tn=%d", i, zVal, tn);
      }
    }
    db_execute("COMMIT");
    ticket_notify(tn, 0, 0, 0);
    cgi_redirect(mprintf("tktview?tn=%d",tn));
    return;
  }else if( P("submit") ){
    if( zTitle[0]==0 ){
      zErrMsg = "Please enter a title.";
    }else if( zDesc[0]==0 ){
      zErrMsg = "Please enter a description.";
    }else if( zContact[0]==0 && g.isAnon ){
      zErrMsg = "Please enter your contact information.";
    }
  }
  
  common_standard_menu("tktnew", 0);
  common_add_help_item("CvstracTicket");
  common_add_action_item( "index", "Cancel");
  common_header("Create A New Ticket");
  if( zErrMsg ){
    @ <blockquote>
    @ <font color="red">%h(zErrMsg)</font>
    @ </blockquote>
  }
  @ <form action="%T(g.zPath)" method="POST">
  @ <table cellpadding="5">
  @
  @ <tr>
  @ <td colspan=2>
  @ Enter a one-line summary of the problem:<br>
  @ <input type="text" name="t" size=70 maxlength=70 value="%h(zTitle)">
  @ </td>
  @ </tr>
  @
  @ <tr>
  @ <td align="right">Type:
  cgi_v_optionmenu2(0, "y", zType, (const char**)db_query(
      "SELECT name, value FROM enums WHERE type='type'"));
  @ </td>
  @ <td>What type of ticket is this?</td>
  @ </tr> 
  @
  @ <tr>
  @   <td align="right"><nobr>
  @     Version: <input type="text" name="v" value="%h(zVers)" size="10">
  @   </nobr></td>
  @   <td>
  @      Enter the version and/or build number of the product
  @      that exhibits the problem.
  @   </td>
  @ </tr>
  @
  @ <tr>
  @   <td align="right"><nobr>
  @     Severity:
  cgi_optionmenu(0, "r", zSev,
         "1", "1", "2", "2", "3", "3", "4", "4", "5", "5", NULL);
  @   </nobr></td>
  @   <td>
  @     How debilitating is the problem?  "1" is a show-stopper defect with
  @     no workaround.  "2" is a major defect with a workaround.  "3"
  @     is a mid-level defect.  "4" is an annoyance.  "5" is a cosmetic
  @     defect or a nice-to-have feature request.
  @   </td>
  @ </tr>
  @
  @ <tr>
  @   <td align="right"><nobr>
  @     Priority:
  cgi_optionmenu(0, "p", zPri,
         "1", "1", "2", "2", "3", "3", "4", "4", "5", "5", NULL);
  @   </nobr></td>
  @   <td>
  @     How quickly do you need this ticket to be resolved?
  @     "1" means immediately.
  @     "2" means before the next build.  
  @     "3" means before the next release.
  @     "4" means implement as time permits.
  @     "5" means defer indefinitely.
  @   </td>
  @ </tr>
  @
  if( g.okWrite ){
    @ <tr>
    @   <td align="right"><nobr>
    @     Assigned To:
    az = db_query("SELECT id FROM user UNION SELECT '' ORDER BY id");
    cgi_v_optionmenu(0, "w", zWho, (const char **)az);
    db_query_free(az);
    @   </nobr></td>
    @   <td>
    @     To what user should this problem be assigned?
    @   </td>
    @ </tr>
    @
    az = db_query("SELECT '', '' UNION ALL "
            "SELECT name, value  FROM enums WHERE type='subsys'");
    if( az[0] && az[1] && az[2] ){
      @ <tr>
      @   <td align="right"><nobr>
      @     Subsystem:
      cgi_v_optionmenu2(4, "s", zSubsys, (const char**)az);
      db_query_free(az);
      @   </nobr></td>
      @   <td>
      @     Which component is showing a problem?
      @   </td>
      @ </tr>
    }
  }
  @ <tr>
  @   <td align="right"><nobr>
  @     Derived From: <input type="text" name="f" value="%h(zFrom)" size="5">
  @   </nobr></td>
  @   <td>
  @      Is this related to an existing ticket?
  @   </td>
  @ </tr>
  if( g.isAnon ){
    @ <tr>
    @   <td align="right"><nobr>
    @     Contact: <input type="text" name="c" value="%h(zContact)" size="20">
    @   </nobr></td>
    @   <td>
    @      Enter a phone number or e-mail address where a developer can
    @      contact you with questions about this ticket.  The information
    @      you enter will be available to the developers only and will not
    @      be visible to general users.
    @   </td>
    @ </tr>
    @
  }
  for(i=1; i<=5; i++){
    char **az;
    const char *zDesc;
    const char *zName;
    char zX[3];
    char zExName[100];

    bprintf(zExName,sizeof(zExName),"extra%d_desc",i);
    zDesc = db_config(zExName, 0);
    if( zDesc==0 ) continue;
    bprintf(zExName,sizeof(zExName),"extra%d_name",i);
    zName = db_config(zExName, 0);
    if( zName==0 ) continue;
    az = db_query("SELECT name, value FROM enums "
                   "WHERE type='extra%d'", i);
    bprintf(zX, sizeof(zX), "x%d", i);
    @ <tr>
    @   <td align="right"><nobr>
    @     %h(zName):
    if( az==0 || az[0]==0 ){
      @     <input type="text" name="%h(zX)" value="%h(PD(zX,""))" size="20">
    }else{
      cgi_v_optionmenu2(0, zX, PD(zX,az[0]), (const char**)az);
    }
    @   </nobr></td>
    @   <td>
    /* description is already HTML markup */
    @      %s(zDesc)
    @   </td>
    @ </tr>
    @
  }
  @ <tr>
  @   <td colspan="2">
  @     Enter a detailed description of the problem.  For code defects,
  @     be sure to provide details on exactly how the problem can be
  @     reproduced.  Provide as much detail as possible. 
  @     <a href="#format_hints">Formatting hints</a>.
  @     <br>
  @ <textarea rows="10" cols="70" wrap="virtual" name="d">%h(zDesc)</textarea>
  if( isPreview ){
    @     <br>Description Preview:
    @     <table border=1 cellpadding=15 width="100%%"><tr><td>
    output_formatted(zDesc, 0);
    @     </td></tr></table>
  }
  if( g.okWrite ){
    @     <br>Note: If you want to include a large script or binary file
    @     with this ticket you will be given an opportunity to add attachments
    @     to the ticket after the ticket has been created.  Do not paste
    @     large scripts or screen dumps in the description.
  }
  @   </td>
  @ </tr>
  @ <tr>
  @   <td align="right">
  @     <input type="submit" name="preview" value="Preview">
  @   </td>
  @   <td>
  @     Preview the formatting of the description.
  @   </td>
  @ </tr>
  @ <tr>
  @   <td align="right">
  @     <input type="submit" name="submit" value="Submit">
  @   </td>
  @   <td>
  @     After filling in the information about, press this button to create
  @     the new ticket.
  @   </td>
  @ </tr>
  @ </table>
  @ </form>
  @ <a name="format_hints">
  @ <hr>
  @ <h3>Formatting Hints:</h3>
  append_formatting_hints();
  common_footer();
}