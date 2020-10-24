void ticket_history(void){
  int tn = 0, rn = 0;
  int lasttn = 0;
  char **az;
  int i;
  char zPage[30];
  const char *zTn;
  time_t orig;
  char zDate[200];
  struct tm *pTm;

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);
  zTn = PD("tn","");
  sscanf(zTn, "%d,%d", &tn, &rn);
  if( tn<=0 ){ cgi_redirect("index"); return; }

  bprintf(zPage,sizeof(zPage),"%d",tn);
  common_standard_menu("tktview", "search?t=1");

  if( rn>0 ){
    common_add_action_item(mprintf("tktview?tn=%d,%d",tn,rn), "View");
  }else{
    common_add_action_item(mprintf("tktview?tn=%d",tn), "View");
  }

  common_add_help_item("CvstracTicket");

  if( g.okWrite ){
    if( rn>0 ){
      common_add_action_item(mprintf("tktedit?tn=%d,%d",tn,rn), "Edit");
    }else{
      common_add_action_item(mprintf("tktedit?tn=%d",tn), "Edit");
    }
    if( attachment_max()>0 ){
      common_add_action_item(mprintf("attach_add?tn=%d",tn), "Attach");
    }
  }
  add_tkt_tools(0,tn);

  /* Get the record from the database.
  */
  db_add_functions();
  az = db_query("SELECT title,origtime,owner FROM ticket WHERE tn=%d", tn);
  if( az == NULL || az[0]==0 ){
    cgi_redirect("index");
    return;
  }

  orig = atoi(az[1]);
  pTm = localtime(&orig);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);

  common_header("Ticket #%d History", tn);
  @ <h2>Ticket %d(tn) History: %h(az[0])</h2>
  @ <ol>
  @ <li>Created %h(zDate) by %h(az[2])</li>

  /* Grab various types of ticket activities from the db.
  ** All must be sorted by ascending time and the first field of each
  ** record should be epoch time. Second field is the record type.
  */
  az = db_query(
    /* Ticket changes
    */
    "SELECT chngtime AS 'time', 1 AS 'type', "
      "user, fieldid, oldval, newval, NULL "
    "FROM tktchng WHERE tn=%d "
    "UNION ALL "

    /* Checkins
    */
    "SELECT chng.date AS 'time', 2 AS 'type', "
       " chng.cn, chng.branch, chng.user, chng.message, chng.milestone "
    "FROM xref, chng WHERE xref.tn=%d AND xref.cn=chng.cn "
    "UNION ALL "

    /* attachments
    */
    "SELECT date AS 'time', 3 AS 'type', atn, size, user, description, fname "
    "FROM attachment WHERE tn=%d "
    "UNION ALL "

    /* inspection reports
    */
    "SELECT inspect.inspecttime AS 'time', 4 AS 'type', "
      "inspect.cn, inspect.inspector, inspect.result, NULL, NULL "
    "FROM xref, inspect "
    "WHERE xref.cn=inspect.cn AND xref.tn=%d "
    "UNION ALL "

    /* derived tickets. This is just the derived ticket creation. Could
    ** also report derived ticket changes, but we'd probably have to
    ** use some kind of tree representation.
    */
    "SELECT origtime AS 'time', 5 AS 'type', tn, owner, title, NULL, NULL "
    "FROM ticket WHERE derivedfrom=%d "

    "ORDER BY 1, 2",
    tn, tn, tn, tn, tn);

  /* find the last ticket change in the list. This is necessary to allow
  ** someone to undo the last change.
  */
  for(i=0; az[i]; i+=7){
    int type = atoi(az[i+1]);
    if( type==1 ) lasttn = i;
  }

  for(i=0; az[i]; i+=7) {
    time_t date = atoi(az[i]);
    int type = atoi(az[i+1]);
    switch( type ){
      case 1: { /* ticket change */
        ticket_change(date, tn, az[i+2],
          az[i+3], az[i+4], az[i+5], lasttn==i);
        break;
      }
      case 2: { /* checkin */
        ticket_checkin(date, atoi(az[i+2]), az[i+3], az[i+4], az[i+5]);
        break;
      }
      case 3: { /* attachment */
        ticket_attach(date, atoi(az[i+2]), atoi(az[i+3]),
          az[i+4], az[i+5], az[i+6]);
        break;
      }
      case 4: { /* inspection report */
        ticket_inspect(date, atoi(az[i+2]), az[i+3], az[i+4]);
        break;
      }
      case 5: { /* derived ticket creation */
        ticket_derived(date, atoi(az[i+2]), az[i+3], az[i+4]);
        break;
      }
      default:
        /* Can't happen */
        /* assert( type >= 1 && type <= 5 ); */
        break;
    }
  }
  @ </ol>
  common_footer();
}