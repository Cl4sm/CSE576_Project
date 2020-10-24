void checkin_view(void){
  char **az, **azFile, **azTkt, **azInspect;
  int cn;
  time_t tx;
  struct tm *pTm;
  int i;
  int cnt;
  char zDate[200];
  char zDateUTC[200];
  int nMs = 0;

  login_check_credentials();
  if( !g.okRead ){ login_needed(); return; }
  throttle(1,0);
  cn = atoi(PD("cn","0"));
  az = db_query("SELECT date, branch, milestone, user, message, directory "
                "FROM chng WHERE cn=%d", cn);
  if( az[0]==0 ){ cgi_redirect("index"); return; }
  azFile = db_query("SELECT filename, vers, nins, ndel, chngtype, prevvers "
                    "FROM filechng WHERE cn=%d ORDER BY +filename", cn);
  azTkt = db_query(
     "SELECT ticket.tn, ticket.title FROM xref, ticket "
     "WHERE xref.cn=%d AND xref.tn=ticket.tn "
     "ORDER BY ticket.tn", cn);
  azInspect = db_query(
     "SELECT inspecttime, inspector, result FROM inspect "
     "WHERE cn=%d ORDER BY inspecttime", cn);
  common_standard_menu("chngview", "search?c=1&f=1");

  nMs = atoi(az[2]);
  if( nMs==0 && g.okWrite && g.okCheckin ){
    common_add_action_item(mprintf("inspect?cn=%d",cn), "Inspection");
  }
  
  /* Show Patchset link only for check-ins.*/
  if( nMs==0 && g.okCheckout ){
    common_add_action_item(mprintf("patchset?cn=%d",cn), "Patchset");
  }
  if( g.okWrite && g.okCheckin ){
    common_add_action_item(mprintf("chngedit?cn=%d",cn), "Edit");
  }
  if( !strcmp(g.scm.zSCM,"cvs") && g.okRead ){
    common_add_action_item(mprintf("taghints?cn=%d",cn), "Tagging/Branching");
  }
  add_chng_tools(0,cn,nMs);
  if( nMs==0 ){
    common_add_help_item("CvstracCheckin");
    common_header("Check-in [%d]", cn);
  }else{
    common_add_help_item("CvstracMilestone");
    common_header("Milestone [%d]", cn);
  }

  tx = (time_t)atoi(az[0]);
  pTm = localtime(&tx);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);
  pTm = gmtime(&tx);
  strftime(zDateUTC, sizeof(zDateUTC), "%Y-%b-%d %H:%M:%S", pTm);

  @ <table cellpadding=1 cellspacing=0 width="100%%">
  @ <tr><td align="right" valign="top">
  if( nMs==0 ){
    @ Check-in Number:
  }else{
    @ Milestone Number:
  }
  @ </td>
  @   <td>%d(cn)</td><td width="30%%"></td></tr>
  @ <tr><td align="right" valign="top">Date:</td>
  @   <td>%s(zDate) (local)
  @   <br>%s(zDateUTC) (UTC)</td></tr>
  @ <tr><td align="right">User:</td><td>%z(format_user(az[3]))</td></tr>
  @ <tr><td align="right">Branch:</td><td>%h(az[1])</td></tr>
  if( nMs && az[5] && az[5][0] ){
    @ <tr><td align="right">Directory:</td>
    @   <td><a href="%h(default_browse_url())?d=%h(az[5])">%h(az[5])</a></td>
    @ </tr>
  }
  if( nMs ){
    @ <tr><td align="right">Type:</td>
    @ <td>%h((nMs==1)?"Release":"Event")</td></tr>
  }
  @ <tr><td align="right" valign="top">Comment:</td><td colspan=2>
  output_formatted(az[4], 0);
  if( g.okWrite && g.okCheckin ){
    @
    @ <a href="chngedit?cn=%d(cn)">(edit)</a>
  }
  @ </td></tr>
  @ <tr><td align="right" valign="top">Tickets:</td><td colspan=2>
  if( azTkt[0]!=0 ){
    @ <table cellpadding=0 cellspacing=0 border=0>
    for(i=0; azTkt[i]; i+=2 ){
      char *z = mprintf("#%s",azTkt[i]);
      @ <tr><td align="right" valign="top">
      output_formatted(z,0);
      @ </td><td width=8></td>
      @ <td>%h(azTkt[i+1])</td></tr>
    }
    @ </table></td></tr>
  }
  @ <tr><td align="right" valign="top">Inspections:</td><td colspan=2>
  for(i=0; azInspect[i]; i+=3){
    tx = (time_t)atoi(azInspect[i]);
    pTm = gmtime(&tx);
    strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S (UTC)", pTm);
    @ %s(zDate) by %h(azInspect[i+1]): %h(azInspect[i+2])<br>
  }
  @ </td></tr>
  @ <tr><td align="right" valign="top">Files:</td><td colspan=2>
  @ <table cellpadding=0 cellspacing=0 border=0>
  for(cnt=i=0; azFile[i]; i+=6, cnt++){
    int chngtype = atoi(azFile[i+4]);
    const char* zVers = azFile[i+1];
    const char* zPrev = azFile[i+5];
    char *zCurP = printable_vers(zVers);
    char *zPrevP = printable_vers(zPrev);
    const char* zF = azFile[i];

    if( g.okCheckout ){
      @ <tr><td><a href="rlog?f=%T(azFile[i])">%h(azFile[i])</a>
    }else{
      @ <tr><td>%h(azFile[i])
    }
    @ &nbsp;&nbsp;&nbsp;&nbsp;</td>
    if( chngtype==1 ){
      @ <td>
      @   added-&gt;<a href="fileview?f=%T(zF)&v=%T(zVers)">
      @   %h(zCurP)</a>
      @ </td>
    }else if( chngtype==2 ){
      @ <td>
      @   <a href="fileview?f=%T(zF)&v=%T(zPrev)">
      @   %h(zPrevP)</a>-&gt;removed
      @ </td>
    }else{
      @ <td>
      @   <a href="fileview?f=%T(zF)&v=%T(zPrev)">%h(zPrevP)</a>
      @   <a href="filediff?f=%T(zF)&v1=%T(zPrev)&v2=%T(zVers)">-&gt;</a>
      @   <a href="fileview?f=%T(zF)&v=%T(zVers)">%h(zCurP)</a>
      @ </td>
      if( azFile[i+2][0] && azFile[i+3][0] ){
        @ <td>&nbsp;&nbsp;&nbsp
        if( atoi(azFile[i+2]) || atoi(azFile[i+3]) ){
          @ %h(azFile[i+2]) inserted, %h(azFile[i+3]) deleted
        }
        @ </td>
      }
    }
    free(zCurP);
    free(zPrevP);
    @ </tr>
  }
  @ </table>
  @ </td></tr></table>
  if( cnt>0 && g.okCheckout ){
    diff_chng(cn,0);
  }
  common_footer();
}