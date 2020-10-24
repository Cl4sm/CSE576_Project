void wiki_page(void){
  const char *pg = P("p");
  const char *zTime = P("t");
  char *zSearch = NULL;
  int doDiff = atoi(PD("diff","0"));
  int tm;
  int i;
  char **azPage;               /* Query result: page to display */
  char **azHist = 0;           /* Query result: history of the page */
  int isLocked;
  char *zTimeFmt;              /* Human readable translation of "t" parameter */
  char *zTruncTime = 0;
  char *zTruncTimeFmt = 0;
  int truncCnt = 0;
  int overload;
  int isHome = 0;
  int isUser = 0;
  int canEdit = 0;
  int canDelete = 0;

  login_check_credentials();
  if( !g.okRdWiki ){ login_needed(); return; }
  overload = throttle(0,0);
  if( overload ){
    zTime = 0;
    doDiff = 0;
  }
  db_add_functions();
  if( zTime==0 || (tm = atoi(zTime))==0 ){
    time_t now;
    time(&now);
    tm = now;
  }
  isHome = is_home_page(pg);
  isUser = isHome || is_user_page(pg);
  if( !isUser && (pg==0 || is_wiki_name(pg)!=strlen(pg)) ){
    pg = "WikiIndex";
  }
  azPage = db_query(
    "SELECT -invtime, locked, who, ipaddr, text "
    "FROM wiki WHERE name='%q' AND invtime>=%d LIMIT 2", pg, -tm
  );
  if( azPage[0]==0 || azPage[5]==0 ){ doDiff = 0; }
  if( zTime && !doDiff ){
    zTimeFmt = db_short_query("SELECT ldate(%d)",tm);
    azHist = db_query(
        "SELECT ldate(-invtime), who, -invtime FROM wiki "
        "WHERE name='%q'", pg
    );
  }

  isLocked = azPage[0] && atoi(azPage[1])!=0;

  if( zTime==0 && !overload ){
    if( g.okAdmin ){
      /* admin or up can always edit */
      canEdit = 1;
    }else if( isHome ){
      /* users can always edit their own "home page" */
      canEdit = 1;
    }else if( !isUser && g.okWiki && !isLocked ){
      /* anyone else with wiki edit can edit unlocked pages, unless they're
      ** some's home page.
      */
      canEdit = 1;
    }
  }

  if( azPage[0] ){
    if( isHome || g.okSetup ){
      canDelete = 1;
    }else if( !isUser && !isLocked
              && ok_to_delete_wiki(atoi(azPage[0]),azPage[2]) ){
      canDelete = 1;
    }
  }

  if( pg && strcmp(pg,"WikiIndex")!=0 ){
    common_standard_menu(0, "search?w=1");
  }else{
    common_standard_menu("wiki", "search?w=1");
  }
  common_add_help_item("CvstracWiki");
  common_add_action_item("wikitoc", "Contents");
  if( canEdit ){
    common_add_action_item( mprintf("wikiedit?p=%t", pg), "Edit");
    if( attachment_max()>0 ){
      common_add_action_item( mprintf("attach_add?tn=%t",pg), "Attach");
    }
  }
  if( zTime==0 && azPage[0] && azPage[5] && !overload ){
    common_add_action_item(mprintf("wiki?p=%t&t=%t", pg, azPage[0]), "History");
  }
  if( !overload ){
    common_add_action_item(mprintf("wiki.txt?p=%t&t=%t", pg, azPage[0]),"Text");
  }
  if( doDiff ){
    common_add_action_item(mprintf("wiki?p=%t&t=%t",pg,azPage[0]), "No-Diff");
  }else if( azPage[0] && azPage[5] ){
    common_add_action_item(mprintf("wiki?p=%t&t=%t&diff=1",pg,azPage[0]),
                           "Diff");
  }
  if( canDelete ){
    const char *zLink;
    if( zTime==0 ){
      zLink = mprintf("wikidel?p=%t", pg);
    }else{
      zLink = mprintf("wikidel?p=%t&t=%d", pg, atoi(azPage[0]));
    }
    common_add_action_item( zLink, "Delete");
  }
  if( azPage[0] ){
    add_wiki_tools(0,pg,atoi(azPage[0]),azPage[5]?atoi(azPage[5]):0);
  }

  zSearch = mprintf("search?s=%t&w=1", pg);
  common_link_header(zSearch,wiki_expand_name(pg));
  if( zTime && !doDiff ){
    @ <table align="right" cellspacing=2 cellpadding=0 border=0
    @  bgcolor="%s(BORDER1)" class="border1">
    @ <tr><td>
    @   <table width="100%%" cellspacing=1 cellpadding=5 border=0
    @    bgcolor="%s(BG1)" class="bkgnd1">
    @   <tr><th bgcolor="%s(BG1)" class="bkgnd1">Page History</th></tr>
    @   </table>
    @ </td></tr>
    @ <tr><td>
    @   <table width="100%%" cellspacing=1 cellpadding=5 border=0
    @    bgcolor="white">
    @   <tr><td>
    for(i=0; azHist[i]; i+=3){
      if( azPage[0] && strcmp(azHist[i+2],azPage[0])==0 ){
        @   <nobr><b>%h(azHist[i]) %h(azHist[i+1])</b><nobr><br>
        if( i>0 && g.okAdmin ){
          zTruncTime = azHist[i+2];
          zTruncTimeFmt = azHist[i];
          truncCnt = 1;
        }
      }else{
        @   <nobr><a href="wiki?p=%h(pg)&amp;t=%h(azHist[i+2])">
        @   %h(azHist[i]) %h(azHist[i+1])</a>&nbsp;&nbsp;&nbsp;</nobr><br>
        if( zTruncTime ) truncCnt++;
      }
    }
    @   <p><nobr><a href="wiki?p=%h(pg)">Turn Off History</a></nobr></p>
    @   </td></tr>
    @   </table>
    @ </td></tr>
    @ </table>
    /* @ <p><big><b>%h(wiki_expand_name(pg))&nbsp;&nbsp;</b></big> */
    /* @ <small><i>(as of %h(zTimeFmt))</i></small></p> */
  }else{
    /* @ <p><big><b>%h(wiki_expand_name(pg))</b></big></p> */
  }
  if( doDiff ){
    diff_strings(3,azPage[9], azPage[4]);
  }else if( azPage[0] ){
    char *zLinkSuffix;
    zLinkSuffix = zTime ? mprintf("&amp;%h",zTime) : "";
    output_wiki(azPage[4], zLinkSuffix, pg);
    isLocked = atoi(azPage[1]);
    attachment_html(pg,
      "<h3>Attachments:</h3>\n<blockquote>",
      "</blockquote>"
    );
  }else{
    if( isHome ){
      @ <i>You haven't written anything on your home page...</i>
    }else if( !isUser ){
      @ <i>This page has not been created...</i>
    }
    isLocked = 0;
  }
  common_footer(); 
}