void setup_newticket_page(void){
  char **azResult;
  const char *zState, *zOldState;
  const char *zAsgnto, *zOldAsgnto;
  const char *zType, *zOldType;
  const char *zPri, *zOldPri;
  const char *zSev, *zOldSev;

  /* The user must be at least the administrator in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okAdmin ){
    login_needed();
    return;
  }

  /*
  ** The "asgnto" query parameter specifies a userid who is assigned to
  ** all new tickets.  Record this value in the configuration table if
  ** it has changed.
  */
  zOldAsgnto = db_config("assignto","");
  zAsgnto = P("asgnto");
  if( zAsgnto && strcmp(zOldAsgnto,zAsgnto)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('assignto','%q');", zAsgnto
    );
    zOldAsgnto = zAsgnto;
    db_config(0,0);
  }

  /*
  ** The "istate" query parameter specifies the initial state for new
  ** tickets.  Record any changes to this value.
  */
  zOldState = db_config("initial_state","");
  zState = P("istate");
  if( zState && strcmp(zOldState,zState)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('initial_state','%q');",
      zState
    );
    zOldState = zState;
    db_config(0,0);
  }

  /*
  ** The "type" query parameter specifies the initial type for new
  ** tickets.  Record any changes to this value.
  */
  zOldType = db_config("dflt_tkt_type","code");
  zType = P("type");
  if( zType && strcmp(zOldType,zType)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('dflt_tkt_type','%q');",
      zType
    );
    zOldType = zType;
    db_config(0,0);
  }

  /*
  ** The "pri" query parameter specifies the initial priority for new
  ** tickets.  Record any changes to this value.
  */
  zOldPri = db_config("dflt_priority","1");
  zPri = P("pri");
  if( zPri && strcmp(zOldPri,zPri)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('dflt_priority','%q');",
      zPri
    );
    zOldPri = zPri;
    db_config(0,0);
  }

  /*
  ** The "sev" query parameter specifies the initial severity for new
  ** tickets.  Record any changes to this value.
  */
  zOldSev = db_config("dflt_severity","1");
  zSev = P("sev");
  if( zSev && strcmp(zOldSev,zSev)!=0 ){
    db_execute(
      "REPLACE INTO config(name,value) VALUES('dflt_severity','%q');",
      zSev
    );
    zOldSev = zSev;
    db_config(0,0);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminNewTicket");
  common_header("Configure New Ticket Defaults");
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Select a user to whom new tickets will be assigned by default:</p><p>
  @ Assigned To:
  azResult = db_query("SELECT id FROM user UNION SELECT '' ORDER BY id");
  cgi_v_optionmenu(0, "asgnto", zOldAsgnto, (const char**)azResult);
  @ </p>
  @
  @ <p>
  @ Select the initial state that new tickets are created in:</p><p>
  @ Initial State:
  cgi_v_optionmenu2(0, "istate", zOldState, (const char**)db_query(
     "SELECT name, value FROM enums WHERE type='status'"));
  @ </p>
  @
  @ <p>
  @ Select the default type for new tickets:</p><p>
  @ Default Type:
  cgi_v_optionmenu2(0, "type", zOldType, (const char**)db_query(
     "SELECT name, value FROM enums WHERE type='type'"));
  @ </p>
  @
  @ <p>
  @ Select the default priority for new tickets:</p><p>
  @ Default Priority:
  cgi_optionmenu(0, "pri", zOldPri, "1", "1", "2", "2", "3", "3", "4", "4",
      "5", "5", NULL);
  @ </p>
  @
  @ <p>
  @ Select the default severity for new tickets:</p><p>
  @ Default Severity:
  cgi_optionmenu(0, "sev", zOldSev, "1", "1", "2", "2", "3", "3", "4", "4",
      "5", "5", NULL);
  @ </p>
  @
  @ <p>
  @ <input type="submit" value="Submit">
  @ </form>
  @ </p>
  common_footer();
}