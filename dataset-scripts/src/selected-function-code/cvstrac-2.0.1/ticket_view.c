void ticket_view(void){
  int i, j, nChng;
  int tn = 0, rn = 0;
  char **az;
  char **azChng;
  char **azDrv;
  char *z;
  const char *azExtra[5];
  char zPage[30];
  const char *zTn;

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
    common_replace_nav_item(mprintf("rptview?rn=%d", rn), "Report");
    common_add_action_item(mprintf("tkthistory?tn=%d,%d", tn, rn), "History");
  }else{
    common_add_action_item(mprintf("tkthistory?tn=%d", tn), "History");
  }
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
  common_add_help_item("CvstracTicket");

  /* Check to see how many "extra" ticket fields are defined
  */
  azExtra[0] = db_config("extra1_name",0);
  azExtra[1] = db_config("extra2_name",0);
  azExtra[2] = db_config("extra3_name",0);
  azExtra[3] = db_config("extra4_name",0);
  azExtra[4] = db_config("extra5_name",0);

  /* Get the record out of the database.
  */
  db_add_functions();
  az = db_query("SELECT "
                "  type,"               /* 0 */
                "  status,"             /* 1 */
                "  ldate(origtime),"    /* 2 */
                "  ldate(changetime),"  /* 3 */
                "  derivedfrom,"        /* 4 */
                "  version,"            /* 5 */
                "  assignedto,"         /* 6 */
                "  severity,"           /* 7 */
                "  priority,"           /* 8 */
                "  subsystem,"          /* 9 */
                "  owner,"              /* 10 */
                "  title,"              /* 11 */
                "  description,"        /* 12 */
                "  remarks, "           /* 13 */
                "  contact,"            /* 14 */
                "  extra1,"             /* 15 */
                "  extra2,"             /* 16 */
                "  extra3,"             /* 17 */
                "  extra4,"             /* 18 */
                "  extra5 "             /* 19 */
                "FROM ticket WHERE tn=%d", tn);
  if( az[0]==0 ){
    cgi_redirect("index");
    return;
  }
  azChng = db_query(
    "SELECT chng.date, chng.cn, chng.branch, chng.user, chng.message, chng.milestone "
    "FROM xref, chng WHERE xref.tn=%d AND xref.cn=chng.cn "
    "ORDER BY chng.milestone ASC, chng.date DESC", tn);
  azDrv = db_query(
    "SELECT tn,title FROM ticket WHERE derivedfrom=%d", tn);
  common_header("Ticket #%d", tn);
  @ <h2>Ticket %d(tn): %h(az[11])</h2>
  @ <blockquote>
  output_formatted(az[12], zPage);
  @ </blockquote>
  @
  @ <table align="right" hspace="10" cellpadding=2 border=0>
  @ <tr><td bgcolor="%h(BORDER1)" class="border1">
  @ <table width="100%%" border=0 cellpadding=4 cellspacing=0>
  @ <tr bgcolor="%h(BG1)" class="bkgnd1">
  @ <td valign="top" align="left">
  if( az[13][0]==0 ){
    @ [<a href="tktappend?tn=%h(zTn)">Add remarks</a>]
  } else {
    @ [<a href="tktappend?tn=%h(zTn)">Append remarks</a>]
  }
  @ </td></tr></table></td></tr></table>
  @ <h3>Remarks:</h3>
  @ <blockquote>
  output_formatted(az[13], zPage);
  @ </blockquote>

  if( az[13][0]!=0 ){
    @ <table align="right" hspace="10" cellpadding=2 border=0>
    @ <tr><td bgcolor="%h(BORDER1)" class="border1">
    @ <table width="100%%" border=0 cellpadding=4 cellspacing=0>
    @ <tr bgcolor="%h(BG1)" class="bkgnd1">
    @ <td valign="top" align="left">
    @ [<a href="tktappend?tn=%h(zTn)">Append remarks</a>]
    @ </td></tr></table></td></tr></table>
    @
  }

  @
  @ <h3>Properties:</h3>
  @ 
  @ <blockquote>
  @ <table>
  @ <tr>
  @   <td align="right">Type:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[0])&nbsp;</b></td>
  @ <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
  @   <td align="right">Version:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[5])&nbsp;</b></td>
  @ </tr>
  @ <tr>
  @   <td align="right">Status:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[1])</b></td>
  @ <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
  @   <td align="right">Created:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[2])</b></td>
  @ </tr>
  @ <tr>
  @   <td align="right">Severity:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[7])&nbsp;</b></td>
  @ <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
  @   <td align="right">Last&nbsp;Change:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[3])</b></td>
  @ </tr>
  @ <tr>
  @   <td align="right">Priority:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[8])&nbsp;</b></td>
  @ <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
  @   <td align="right">Subsystem:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[9])&nbsp;</b></td>
  @ </tr>
  @ <tr>
  @   <td align="right">Assigned&nbsp;To:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[6])&nbsp;</b></td>
  @ <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
  @   <td align="right">Derived From:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>
  z = extract_integer(az[4]);
  if( z && z[0] ){
    z = mprintf("#%s",z);
    output_formatted(z,zPage);
  }else{
    @   &nbsp;
  }
  @   </b></td>
  @ </tr>
  @ <tr>
  @   <td align="right">Creator:</td>
  @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[10])&nbsp;</b></td>
  if( g.okWrite && !g.isAnon ){
    @ <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
    @   <td align="right">Contact:</td>
    if( strchr(az[14],'@') ){
      @   <td bgcolor="%h(BG3)" class="bkgnd3"><b><a href="mailto:%h(az[14])">
      @        %h(az[14])</a>&nbsp;</b></td>
    }else{
      @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[14])&nbsp;</b></td>
    }
    @ </tr>
    j = 0;
  } else {
    j = 1;
  }
  for(i=0; i<5; i++){
    if( azExtra[i]==0 ) continue;
    if( j==0 ){
      @ <tr>
    }else{
      @ <td></td>
    }
    @   <td align="right">%h(azExtra[i]):</td>
    @   <td bgcolor="%h(BG3)" class="bkgnd3"><b>%h(az[15+i])&nbsp;</b></td>
    if( j==0 ){
      j = 1;
    }else{
      @ </tr>
      j = 0;
    }
  }
  if( j==1 ){
    @ </tr>
  }
  @ <tr>
  @ </tr>
  @ </table>
  @ </blockquote>
  if( azDrv[0] ){
    int i;
    @ <h3>Derived Tickets:</h3>
    @ <table cellspacing=0 border=0 cellpadding=0>
    for(i=0; azDrv[i]; i+=2){
      @ <tr><td valign="top" width=160 align="right">
      z = mprintf("#%s",azDrv[i]);
      output_formatted(z,zPage);
      @ </td>
      @ <td valign="center" width=30 align="center">
      common_icon("ptr1");
      @ </td>
      @ <td valign="top" align="left">
      output_formatted(azDrv[i+1],0);
      @ </td></tr>
    }
    @ </table>
  }
  nChng = 0;
  if( azChng[0] && azChng[5] && atoi(azChng[5])==0 ){
    int i;
    @ <h3>Related Check-ins:</h3>
    @ <table cellspacing=0 border=0 cellpadding=0>
    for(i=0; azChng[i]; i+=6){
      /* Milestones are handeld in loop below */
      if( atoi(azChng[i+5]) ) break;

      nChng++;
      output_tkt_chng(&azChng[i]);
    }
    @ </table>
  }
  
  if( azChng[0] && azChng[nChng*6] ){
    int i;
    @ <h3>Related Milestones:</h3>
    @ <table cellspacing=0 border=0 cellpadding=0>
    for(i=nChng*6; azChng[i]; i+=6){
      output_tkt_chng(&azChng[i]);
    }
    @ </table>
  }
  attachment_html(zPage,"<h3>Attachments:</h3>\n<blockquote>","</blockquote>");
  common_footer();
}