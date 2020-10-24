void timeline_page(void){
  const char *zEnd;       /* Day at which timeline ends */
  time_t begin, end;      /* Beginning and ending times for the timeline */
  char **az;  
  time_t thisDate;
  int thisDay, lastDay;
  int inTable = 0;
  int i;
  struct tm *pTm;
  int len = 0;
  int days = 30;
  int showM;       /* Show milestones if 1.  Do not show if 0 */
  int showC;       /* Show trunk-only if 3, all checkins if 2,
                   ** branch checkins if 1, none if 0 */
  int showS;       /* 0: no status changes 1: active & new
                   ** 2: all status chnganges 3: everything
                   ** 9: show no tickets*/
  int showA;       /* Show assignment changes if 1 */
  int showW;       /* Show changes to Wiki pages if 1 */
  int showT;       /* Show attachment additions */
  int divM;        /* Divide timeline by milestones if 1 */
  int divT;        /* Divide timeline by days if 1 */
  const char *zCkinPrfx;   /* Only show checkins of files with this prefix */
  char zDate[200];
  char zSQL[4000];
  char *zTimelineCookieName = mprintf("%t_timeline",g.zName);

  login_check_credentials();
  if( !g.okRead && !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  history_update(0);
  if( P("x") || P(zTimelineCookieName) ){

    /* load in the contents of the cookie first, if possible. This gives us a
    ** "blend" of user-specific defaults and things in the query string. x=1 is
    ** set not only for form submission, but links from other pages (i.e.
    ** /dir's "Activity" link), so it's important that we combine them nicely.
    */
    if( P("x")==0 && P(zTimelineCookieName) ){
      char *zName;
      char *zValue;
      char *z = strdup(PD(zTimelineCookieName,""));

      /* The cookie is formatted as a standard CGI query string. What we can
      ** do is parse through it at set values as parameters. This code is
      ** basically add_param_list() from cgi.c except that we don't override
      ** existing parameters since we don't want to stomp on values in the
      ** query string.
      */
      while( *z ){
        while( isspace(*z) ){ z++; }
        zName = z;
        while( *z && *z!='=' && *z!='&' ){ z++; }
        if( *z=='=' ){
          *z = 0;
          z++;
          zValue = z;
          while( *z && *z!='&' ){ z++; }
          if( *z ){
            *z = 0;
            z++;
          }
          dehttpize(zValue);
          if( P(zName)==0 ) cgi_set_parameter(zName, zValue);
        }else{
          if( *z ){ *z++ = 0; }
          if( P(zName)==0 ) cgi_set_parameter(zName, "");
        }
      }
    }

    /* If form is submited, store all timeline settings in cookie and format 
    ** timeline according to values from submited form. 
    */
    if( P("x") ){
      /* The "timeline_cookie_life" value is in days, and we'll use 90 by
      ** default.  Setting it to zero will, of course, disable timeline
      ** cookies.
      */
      int nCookieLife = 86400*atoi(db_config("timeline_cookie_life","90"));
      if( nCookieLife ){
        char *zCookie = mprintf(
          "d=%d&m=%d&c=%d&s=%d&a=%d&w=%d&t=%d&dm=%d&dt=%d",
          atoi(PD("d","30")), atoi(PD("m","0")), atoi(PD("c","0")),
          atoi(PD("s","0")),  atoi(PD("a","0")), atoi(PD("w","0")),
          atoi(PD("t","0")), atoi(PD("dm","0")), atoi(PD("dt","0")));
        cgi_set_cookie(zTimelineCookieName, zCookie, 0, nCookieLife);
      }
    }

    showM = PD("m","0")[0] - '0';
    showC = PD("c","0")[0] - '0';
    showS = PD("s","0")[0] - '0';
    showA = PD("a","0")[0] - '0';
    showW = PD("w","0")[0] - '0';
    showT = PD("t","0")[0] - '0';
    divM = PD("dm","0")[0] - '0';
    divT = PD("dt","0")[0] - '0';

    if( P("set") && P("d") && g.okAdmin ){
      char zVal[12];
      zVal[0] = showA + '0';
      zVal[1] = showC + '0';
      zVal[2] = showM + '0';
      zVal[3] = showS + '0';
      zVal[4] = showW + showT*2 + '0';
      zVal[5] = divM + divT*2 + '0';
      zVal[6] = 0;
      db_execute("REPLACE INTO config(name,value) "
                 "VALUES('timeline_format','%q%q')", zVal, P("d"));
      db_config(0,0);
    }
  }else{
    const char *zFormat;
    zFormat = db_config("timeline_format", 0);
    if( zFormat && zFormat[0] ){
      showA = zFormat[0]-'0';
      showC = zFormat[1]-'0';
      showM = zFormat[2]-'0';
      showS = zFormat[3]-'0';
      showW = zFormat[4]-'0';
      showT = (showW>>1)&1;
      showW &= 1;
      divM = zFormat[5]-'0';
      divT = divM/2;
      divM %= 2;
      days = atoi(&zFormat[6]);
    }else{
      showM = showS = 1;
      showC = 2;
      showA = showW = showT = 0;
      divM = 0;
      divT = 1;
    }
  }
  zCkinPrfx = P("px");
  if( zCkinPrfx==0 || zCkinPrfx[0]==0 || showC==0 ){
    zCkinPrfx = 0;
  }else{
    zCkinPrfx = sqlite3_mprintf("%q", zCkinPrfx);
  }
  if( !g.okRead ){
    showS = showA = 0;
  }
  zEnd = P("e");
  if( zEnd==0 || strcmp(zEnd,"today")==0 || (end = parse_time(zEnd))==0 ){
    time(&end);
  }
  pTm = localtime(&end);
  pTm->tm_hour = 0;
  pTm->tm_min = 0;
  pTm->tm_sec = 0;
  end = mktime(pTm);
  i = atoi(PD("d","0"));
  if( i>0 ) days = i;
  begin = end - 3600*24*days;
  end += 3600*24 - 1;
  if( showM || (showC && zCkinPrfx==0) || divM ){
    appendf(zSQL,&len,sizeof(zSQL),
       "SELECT "
       "  date AS 'time', 1 AS 'type', user, milestone, branch, cn, message "
       "FROM chng WHERE date<=%d AND date>=%d",
       (int)end, (int)begin);
    if( showM==0 && divM==0 ){
      appendf(zSQL,&len,sizeof(zSQL)," AND NOT milestone");
    }
    if( showC==0 || zCkinPrfx  ){
      appendf(zSQL,&len,sizeof(zSQL)," AND milestone%s", showM ? "" : "==1");
    }else if( showC!=2 ){
      appendf(zSQL,&len,sizeof(zSQL)," AND (milestone%s OR branch%c='')",
              showM ? "":"==1", (showC==3) ? '=' : '!');
    }else if( showM==0 && divM ){
      /* we need to know where releases are, but we won't show them. We also
      ** want checkins since showC!=0 at this point.
      */
      appendf(zSQL,&len,sizeof(zSQL)," AND milestone<=1");
    }
    if( showC && zCkinPrfx==0 ){
      appendf(zSQL,&len,sizeof(zSQL),
         " UNION ALL "
         "SELECT "
         "  inspect.inspecttime AS 'time',"
         "  8 AS type,"
         "  inspect.inspector,"
         "  0 AS milestone,"
         "  chng.branch,"
         "  inspect.cn,"
         "  inspect.result "
         "FROM inspect, chng "
         "WHERE inspect.cn=chng.cn "
           "AND inspect.inspecttime<=%d AND inspect.inspecttime>=%d ",
         (int)end, (int)begin);
      if( showC!=2 ){
        appendf(zSQL,&len,sizeof(zSQL)," AND branch%c=''",
                (showC==3) ? '=' : '!');
      }
    }
  }
  if( zCkinPrfx ){
    if( len>0 ){
      appendf(zSQL,&len,sizeof(zSQL)," UNION ALL ");
    }
    appendf(zSQL,&len,sizeof(zSQL),
       "SELECT DISTINCT "
       "  chng.date AS 'time',"
       "  1 AS type,"
       "  chng.user,"
       "  0 AS milestone,"
       "  chng.branch,"
       "  chng.cn,"
       "  chng.message "
       "FROM chng, filechng "
       "WHERE filechng.cn=chng.cn AND filechng.filename LIKE '%.100s%%'"
       "  AND chng.date<=%d AND chng.date>=%d ",
       zCkinPrfx, (int)end, (int)begin);

    appendf(zSQL,&len,sizeof(zSQL),
       " UNION ALL "
       "SELECT DISTINCT "
       "  inspect.inspecttime AS 'time',"
       "  8 AS type,"
       "  inspect.inspector,"
       "  0 AS milestone,"
       "  chng.branch,"
       "  inspect.cn,"
       "  inspect.result "
       "FROM inspect, chng, filechng "
       "WHERE inspect.cn=chng.cn AND filechng.cn=chng.cn "
         "AND filechng.filename LIKE '%.100s%%' "
         "AND inspect.inspecttime<=%d AND inspect.inspecttime>=%d ",
       zCkinPrfx, (int)end, (int)begin);
  }
  if( (showS || showA) && showS!=9 ){
    if( len>0 ){
      appendf(zSQL,&len,sizeof(zSQL)," UNION ALL ");
    }
    appendf(zSQL,&len,sizeof(zSQL),
       "SELECT origtime AS 'time', 2 AS 'type', owner, type, NULL, tn, title "
       "FROM ticket WHERE origtime<=%d AND origtime>=%d",
       (int)end, (int)begin);
    if( showS ){
      appendf(zSQL,&len,sizeof(zSQL),
         " UNION ALL "
         "SELECT chngtime AS 'time', 3 AS 'type', user, oldval AS 'aux1', "
         "       newval AS 'aux2', tn AS 'ref', NULL as 'text' "
         "FROM tktchng "
         "WHERE fieldid='status' AND chngtime<=%d AND chngtime>=%d ",
         (int)end, (int)begin);
      if( showS==1 ){
        appendf(zSQL,&len,sizeof(zSQL),
          " AND ("
          "(newval IN ('new','active') AND oldval NOT IN ('new','active')) OR"
          "(newval NOT IN ('new','active') AND oldval IN ('new','active')))");
      }
    }
    if( showS==3 ){
      appendf(zSQL,&len,sizeof(zSQL),
         " UNION ALL "
         "SELECT chngtime AS 'time', 6 AS 'type', user, '' AS 'aux1', "
         "       '' AS 'aux2', tn AS 'ref', NULL as 'text' "
         "FROM tktchng "
         "WHERE fieldid!='status' AND fieldid!='assignedto'"
         "  AND chngtime<=%d AND chngtime>=%d ",
         (int)end, (int)begin);
    }
    if( showA ){
      appendf(zSQL,&len,sizeof(zSQL),
         " UNION ALL "
         "SELECT chngtime AS 'time', 4 AS 'type', user, oldval AS 'aux1', "
         "       newval AS 'aux2', tn AS 'ref', NULL as 'text' "
         "FROM tktchng "
         "WHERE fieldid='assignedto' AND chngtime<=%d AND chngtime>=%d",
         (int)end, (int)begin);
    }
  }
  if( showW ){
    if( len>0 ){
      appendf(zSQL,&len,sizeof(zSQL)," UNION ALL ");
    }
    appendf(zSQL,&len,sizeof(zSQL),
       "SELECT -invtime AS 'time', 5 AS 'type', who, NULL, NULL, "
       "       name as 'ref', NULL as 'text' "
       "FROM wiki "
       "WHERE invtime>=%d AND invtime<=%d",
       -(int)end, -(int)begin);
  }
  if( showT ){
    if( len>0 ){
      appendf(zSQL,&len,sizeof(zSQL)," UNION ALL ");
    }
    appendf(zSQL,&len,sizeof(zSQL),
       "SELECT date AS 'time', 7 AS 'type', user, tn, size, "
       "       fname as 'ref', atn as 'text' "
       "FROM attachment "
       "WHERE date>=%d AND date<=%d AND tn>0",
       (int)begin, (int)end);
  }

  if( len==0 ){
    static char *azDummy[] = { 0 };
    az = azDummy;
  }else{
    appendf(zSQL,&len,sizeof(zSQL), " ORDER BY 1 DESC, 2");
    az = db_query("%s",zSQL);
  }

  lastDay = 0;
  common_standard_menu("timeline", "search?t=1&c=1");

  common_add_action_item(
      mprintf( "timeline.rss?d=%d&px=%T", days, zCkinPrfx),
      "RSS"
  );
 
  common_add_help_item("CvstracTimeline");

  if( zCkinPrfx && zCkinPrfx[0]
      && db_exists("SELECT dir FROM file WHERE dir='%q'", zCkinPrfx)
  ){
    common_add_action_item( mprintf("%s?d=%h",default_browse_url(),zCkinPrfx),
                            "Directory" );
  }

  common_header("Timeline");
  if( P("debug1") ){
    @ <p>%h(zSQL)</p><hr>
  }
  for(i=0; az[i]; i+=7){
    char *zIcon = 0;
    char *zBg = "";
    char *zFg = 0;
    char *zMsg = 0; /* plain message */
    char *zWiki = 0;  /* wiki-formatted message */
    char *zAttach = 0;
    char zPrefix[1400];
    char zSuffix[1200];
    int nEdits, nLastEdit;
    
    thisDate = atoi(az[i]);
    pTm = localtime(&thisDate);
    thisDay = timet_to_day(thisDate);
    if( !inTable ){
      @ <table cellspacing=0 border=0 cellpadding=0>
      inTable = 1;
    }
    if( thisDay!=lastDay && divT ){
      strftime(zDate, sizeof(zDate), "%A, %Y-%b-%d", pTm);
      @ <tr><td colspan=3>
      @ <table cellpadding=2 border=0>
      @ <tr><td bgcolor="%s(BORDER1)" class="border1">
      @ <table cellpadding=2 cellspacing=0 border=0><tr>
      @   <td bgcolor="%s(BG1)" class="bkgnd1">%s(zDate)</td>
      @ </tr></table>
      @ </td></tr></table>
      @ </td></tr>
      lastDay = thisDay;
    }
    if( divM && az[i+1][0]=='1' && az[i+3][0] && az[i+3][0]=='1' ){
      @ <tr><td colspan=3>
      @ <table cellpadding=2 border=0>
      @ <tr><td bgcolor="%s(BORDER2)" class="border2">
      @ <table cellpadding=2 cellspacing=0 border=0><tr>
      if( az[i+4] && az[i+4][0] ){
        @   <td bgcolor="%s(BG2)" class="bkgnd2">%h(az[i+6])
        @       (<i>%h(az[i+4])</i>)</td>
      } else {
        @   <td bgcolor="%s(BG2)" class="bkgnd2">%h(az[i+6])</td>
      }
      @ </tr></table>
      @ </td></tr></table>
      @ </td></tr>
      if( !divT ) lastDay = thisDay-1;
    }
    if( az[i+1][0]=='1' && !showM && az[i+3][0] && az[i+3][0]!='0' ) continue;
    if( divT || thisDay==lastDay ){
      strftime(zDate, sizeof(zDate), "%H:%M", pTm);
    }else{
      strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M", pTm);
      lastDay = thisDay;
    }
    @ <tr>
    @ <td valign="top" align="right" width=%d(160-divT*100)>%s(zDate)</td>
    zPrefix[0] = 0;
    zSuffix[0] = 0;
    zAttach = 0;
    switch( atoi(az[i+1]) ){
      case 1: { /* A check-in or milestone */
        zWiki = az[i+6];
        if( az[i+3][0] && az[i+3][0]!='0' ){
          zIcon = "box";
          bprintf(zPrefix, sizeof(zPrefix), "Milestone [%.20s]: ", az[i+5]);
        }else{
          zIcon = "dot";
          if( az[i+4][0] ){
            bprintf(zPrefix, sizeof(zPrefix),
                    "Check-in [%.20s] on branch %.50s: ",
                    az[i+5], az[i+4]);
            if( showC==2 ) zBg = " bgcolor=\"" BG4 "\" class=\"bkgnd5\"";
          }else{
            bprintf(zPrefix, sizeof(zPrefix), "Check-in [%.20s]: ", az[i+5]);
          }
          bprintf(zSuffix, sizeof(zSuffix), " (By %z)", format_user(az[i+2]));
        }
        break;
      }
      case 2: {  /* A new ticket was created */
        zMsg = az[i+6];
        bprintf(zPrefix, sizeof(zPrefix), "Create ticket #%.20s: ", az[i+5]);
        bprintf(zSuffix, sizeof(zSuffix), " (By %z)", format_user(az[i+2]));
        zIcon = strcmp(az[i+3],"code")==0 ? "x" : "ptr1";
        break;
      }
      case 3: {  /* The status field of a ticket changed */
        char zType[50];
        bprintf(zType,sizeof(zType),"%.30s",az[i+4]);
        if( islower(zType[0]) ) zType[0] = toupper(zType[0]);
        bprintf(zPrefix, sizeof(zPrefix), "%.30s ticket #%.20s, was %.20s.",
             zType, az[i+5], az[i+3]);
        bprintf(zSuffix, sizeof(zSuffix), " (By %z)", format_user(az[i+2]));
        if( isActiveState(az[i+4]) ){
          zIcon = "x";
        }else if( isFixedState(az[i+4]) ){
          zIcon = "ck";
        }else{
          zIcon = "dia";
        }
        if( az[i+7] && atoi(az[i+8])==4 && strcmp(az[i],az[i+7])==0
            && strcmp(az[i+5],az[i+12])==0 ){
          i += 7;
          if( az[i+4][0]==0 ){
            appendf(zPrefix,0,sizeof(zPrefix), " Unassign from %.50s.", az[i+3]);
          }else if( az[i+3][0]==0 ){
            appendf(zPrefix,0,sizeof(zPrefix), " Assign to %.50s.", az[i+4]);
          }else{
            appendf(zPrefix,0,sizeof(zPrefix), " Reassign from %.50s to %.50s",
                    az[i+3], az[i+4]); 
          }
        }
        if( az[i+7] && atoi(az[i+8])==6 && strcmp(az[i],az[i+7])==0
            && strcmp(az[i+5],az[i+12])==0 ){
          i += 7;
          appendf(zPrefix,0,sizeof(zPrefix), " Plus other changes.");
          while( az[i+7] && atoi(az[i+8])==6 && strcmp(az[i],az[i+7])==0
                 && strcmp(az[i+5],az[i+12])==0 ){
            i += 7;
          }
        }
        break;
      }
      case 4: {  /* The assigned-to field of a ticket changed */
        if( az[i+4][0]==0 ){
          bprintf(zPrefix, sizeof(zPrefix), "Unassign ticket #%.20s from %.50s.",
             az[i+5], az[i+3]);
        }else if( az[i+3][0]==0 ){
          bprintf(zPrefix, sizeof(zPrefix), "Assign ticket #%.20s to %.50s.",
             az[i+5], az[i+4]);
        }else{
          bprintf(zPrefix, sizeof(zPrefix),
             "Reassign ticket #%.20s from %.50s to %.50s",
             az[i+5], az[i+3], az[i+4]); 
        }
        bprintf(zSuffix, sizeof(zSuffix), " (By %z)", format_user(az[i+2]));
        zIcon = "arrow";
        break;
      }
      case 5: {   /* Changes to a Wiki page */
        bprintf(zPrefix, sizeof(zPrefix), "Wiki page {wiki:%s} ", az[i+5]);
        zIcon = "star";
        /* Skip over subsequent lines of same text and display number 
        ** of edits if > 1. Only collapse items from same day.
        */
        nEdits = 1;
        while( az[i+7] && atoi(az[i+8])==5 && strcmp(az[i+5],az[i+12])==0
               && strcmp(az[i+2],az[i+9])==0 ){
          if( divT && !same_day(atoi(az[i]), atoi(az[i+7])) ){
            break;
          }
          i += 7;
          nEdits++;
        }
        if( nEdits>1 ){
          bprintf(zSuffix, sizeof(zSuffix), "edited %d times by %z", nEdits, 
                  format_user(az[i+2]));
        }else{
          bprintf(zSuffix, sizeof(zSuffix), "edited by %z", format_user(az[i+2]));
        }
        appendf(zSuffix,0,sizeof(zSuffix),
                " <a href=\"wiki?p=%T&t=%d&diff=1\">(diff)</a>",
                az[i+5], thisDate);
        break;
      }
      case 6: {  /* Changes to a ticket other than status or assignment */
        bprintf(zSuffix, sizeof(zSuffix), " (By %z)", format_user(az[i+2]));
        zIcon = "arrow";
        /* Skip over subsequent lines of same text and display number 
        ** of edits if > 1. Only collapse items from same day.
        */
        nEdits = 1;
        nLastEdit = atoi(az[i]);
        while( az[i+7] && atoi(az[i+8])==6 && strcmp(az[i+5],az[i+12])==0 
               && strcmp(az[i+2],az[i+9])==0 ){
          if( divT && !same_day(atoi(az[i]), atoi(az[i+7])) ){
            break;
          }
          if( atoi(az[i+7])!=nLastEdit ){
            nLastEdit = atoi(az[i+7]);
            nEdits++;
          }
          i += 7;
        }
        if( nEdits>1 ){
          bprintf(zPrefix, sizeof(zPrefix), "%d changes to ticket #%.20s",
                  nEdits, az[i+5]);
        }else{
          bprintf(zPrefix, sizeof(zPrefix), "Changes to ticket #%.20s", az[i+5]);
        }
        break;
      }
      case 7: { /* Attachments */
        if( isdigit(az[i+3][0]) ){
          bprintf(zPrefix, sizeof(zPrefix), "Attachment to ticket #%.20s: ",
                  az[i+3]);
        }else{
          bprintf(zPrefix, sizeof(zPrefix), "Attachment to {wiki:%.100s}: ",
                  az[i+3]);
        }
        zAttach = mprintf(
            "%h bytes <a href=\"attach_get/%T/%T\">%h</a>",
            az[i+4], az[i+6], az[i+5], az[i+5]);
        bprintf(zSuffix, sizeof(zSuffix), "(By %z)", format_user(az[i+2]));
        zIcon = "arrow";
        break;
      }
      case 8: { /* An inspection */
        zMsg = az[i+6];
        zIcon = "dot";
        if( az[i+4][0] ){
          bprintf(zPrefix, sizeof(zPrefix),
                  "Inspection of [%.20s] on branch %.50s: ",
                  az[i+5], az[i+4]);
          if( showC==2 ) zBg = " bgcolor=\"" BG4 "\" class=\"bkgnd5\"";
        }else{
          bprintf(zPrefix, sizeof(zPrefix), "Inspection of [%.20s]: ", az[i+5]);
        }
        bprintf(zSuffix, sizeof(zSuffix), " (By %z)", format_user(az[i+2]));
        break;
      }
      default:
        /* Cannot happen */
        break;
    }
    @ <td valign="top" align="center" width=30>
    if( zIcon ){
      common_icon(zIcon);
    }
    @ </td>
    @ <td valign="top"%s(zBg)>
    if( zFg ){
      @ <font color="%s(zFg)">
    }
    assert( strlen(zPrefix)<sizeof(zPrefix)-1000 );
    output_formatted(zPrefix, 0);
    if( zWiki ){
      if( output_trim_message(zWiki, MN_CKIN_MSG, MX_CKIN_MSG) ){
        output_formatted(zWiki, 0);
        @ &nbsp;[...]
      }else{
        output_formatted(zWiki, 0);
      }
    }else if(zMsg){
      @ %h(zMsg)
    }
    if( zAttach ){
      @ %s(zAttach)
      free(zAttach);
      zAttach = 0;
    }
    cgi_printf("%s", zSuffix);  /* suffix is always user info */
    if( zFg ){
      @ </font>
    }
    @ </td></tr>
  }
  if( inTable ){
    @ </table>
    inTable = 0;
  }
  @ <hr>
  @ <form method="GET" action="timeline">
  @ <table cellpadding=3>
  @ <tr><td colspan=2>
  @ Show a timeline of 
  cgi_text("d", 0, 0, 0, 0, 5, 8, 0, mprintf("%d",days), 0);
  @ days going backwards from
  pTm = localtime(&end);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d", pTm);
  cgi_text("e", 0, 0, 0, 0, 14, 50, 0, mprintf("%s",zDate), 0);
  @ </td></tr>
  @ <tr>
  zCkinPrfx = P("px");
  if( g.okCheckout ){
    @ <td align="left" valign="top">
    @ <fieldset><legend>Check-in options</legend>
    cgi_radio_fieldset(0, "c", 0, 0, mprintf("%d",showC),
      "2", 0, "Show all check-ins<br>",
      "3", 0, "Show only trunk check-ins<br>",
      "1", 0, "Show only branch check-ins<br>",
      "0", 0, "Show no check-ins<br>",
      NULL
    );
    @ Only show check-ins of files with this prefix:
    cgi_text("px", 0, 0, 0, 0, 25, 0, 0,
      mprintf("%s",zCkinPrfx?zCkinPrfx:""), 0);
    @ </fieldset>
    @ </td>
  }else{
    cgi_hidden("c", 0, mprintf("%d",showC));
    cgi_hidden("px", 0, mprintf("%s",zCkinPrfx?zCkinPrfx:""));
  }
  if( g.okRead ){
    @ <td valign="top">
    cgi_radio_fieldset("Ticket options", "s", 0, 0, mprintf("%d",showS),
      "3", 'A', "Show all ticket changes of any kind<br>",
      "2", 'S', "Show all ticket status changes<br>",
      "1", 'N', "Show only &quot;active&quot; and &quot;new&quot; "
                "status changes<br>",
      "0", 'U', "Show no ticket status changes<br>",
      "9", 'H', "Show no tickets at all<br>",
      NULL
    );
    @ </td>
  }else{
    cgi_hidden("s", 0, mprintf("%d",showS));
  }
  @ </tr>
  @ <tr><td valign="top">
  cgi_checkbox_fieldset("Timeline options", 0, 0,
    "dm", "1", 0, mprintf("%d",divM), "Divide timeline by milestones<br>",
    "dt", "1", 0, mprintf("%d",divT), "Divide timeline by days<br>",
    "debug1", "1", 0, mprintf("%s",P("debug1")),
      "Show the SQL used to generate the timeline",
    NULL
  );
  @ <br>
  cgi_hidden("x", 0, "1");
  cgi_submit(0, 0, 0, 0, 0, "Show Timeline");
  if( g.okAdmin ){
    @ &nbsp;&nbsp;&nbsp;
    cgi_submit("set", 0, 0, 0, 0, "Make Default");
  }
  @ </td><td valign="top">
  @ <fieldset><legend>Other options</legend>
  if( g.okRead ){
    cgi_checkbox("a", 0, 0, 0, 0, 0, "1", mprintf("%d",showA),
      "Show assignment changes<br>");
  }else{
    cgi_hidden("a", 0, mprintf("%d",showA));
  }
  if( g.okCheckout ){
    cgi_checkbox("m", 0, 0, 0, 0, 0, "1", mprintf("%d",showM),
      "Show milestones<br>");
  }else{
    cgi_hidden("m", 0, mprintf("%d",showM));
  }
  if( g.okRdWiki ){
    cgi_checkbox("w", 0, 0, 0, 0, 0, "1", mprintf("%d",showW),
      "Show Wiki edits<br>");
  }else{
    cgi_hidden("w", 0, mprintf("%d",showW));
  }
  if( g.okRdWiki || g.okRead ){
    cgi_checkbox("t", 0, 0, 0, 0, 0, "1", mprintf("%d",showT),
      "Show attachments<br>");
  }else{
    cgi_hidden("t", 0, mprintf("%d",showT));
  }
  @ </fieldset>
  @ </td></tr>
  @ </table>
  @ </form>
  common_footer();
}