static void revision_history(const char *zName, int showMilestones){
  char **az;
  int i;
  const char *zTail;

  if( zName[0]=='/' ) zName++;  /* Be nice to TortoiceCVS */
  zTail = strrchr(zName, '/');
  if( zTail ) zTail++;

  /* @ <h2>History of /%h(zName)</h2> */
  if( showMilestones ){
    common_add_action_item(mprintf("rlog?f=%t",zName), "Omit Milestones");
    az = db_query("SELECT filechng.cn, date, vers, nins, ndel, prevvers,"
                  "       message, user, branch "
                  "FROM filechng, chng "
                  "WHERE filename='%q' AND filechng.cn=chng.cn "
                  "UNION ALL "
                  "SELECT '',date,cn,NULL,NULL,NULL,message,user,branch "
                  "FROM chng "
                  "WHERE milestone=1 "
                  "ORDER BY 2 DESC", zName);
  } else {
    common_add_action_item(mprintf("rlog?f=%t&sms=1",zName), "Show Milestones");
    az = db_query("SELECT filechng.cn, date, vers, nins, ndel, prevvers,"
                  "       message, user, branch "
                  "FROM filechng, chng "
                  "WHERE filename='%q' AND filechng.cn=chng.cn "
                  "ORDER BY date DESC", zName);
  }

  common_header("History for /%h", zName);

  @ <table cellpadding=0 cellspacing=0 border=0>
  for(i=0; az[i]; i+=9){
    time_t t;
    struct tm *pTm;
    char zDate[100];

    t = atoi(az[i+1]);
    pTm = localtime(&t);
    strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M", pTm);
    if( i==0 ){
      @ <thread><tr><th>Date</th><th width=80>Version</th>
      @         <th>Description</th></tr>
      @ <tbody>
    }
    if( i%2 ){
      @ <tr bgcolor="%s(BG4)" class="bkgnd4">
    }else{
      @ <tr>
    }
    @ <td align="right" valign="top"><nobr>%s(zDate)</nobr></td>
    if( az[i][0]==0 ){
      @ <td align="center" valign="top">
      common_icon("box");
      @ </td>
      if( az[i+8] && az[i+8][0] ){
        @ <td align="left" bgcolor="%s(BG5)" class="bkgnd5">
        @ Milestone
        output_chng(atoi(az[i+2]));
        @    on branch %h(az[i+8]):
      }else{
        @ <td align="left">
        @ Milestone
        output_chng(atoi(az[i+2]));
      }
    }else{
      @ <td valign="top" align="center">&nbsp;&nbsp;
      @ <a href="fileview?f=%T(zName)&v=%T(az[i+2])">
      @    %h(printable_vers(az[i+2]))</a>
      @ &nbsp;&nbsp;</td>
      if( az[i+8] && az[i+8][0] ){
        @ <td bgcolor="%s(BG5)" class="bkgnd5">Check-in
        output_chng(atoi(az[i]));
        @     on branch %h(az[i+8]):
      }else{
        @ <td>Check-in
        output_chng(atoi(az[i]));
        @ :
      }
    }
    output_formatted(az[i+6], 0);
    @&nbsp;By %z(format_user(az[i+7])).
    if( az[i][0]!=0 ){ /* Can't diff a Milestone */
      if( g.okCheckout && az[i+5] && az[i+5][0] ){
        @ <a href="filediff?f=%T(zName)&v1=%T(az[i+5])&v2=%T(az[i+2])">
        @ (diff)</a>
      }
    }
    @ </td></tr>
  }
  if( i==0 ){
    @ <tr><td>Nothing is known about this file</td></tr>
  }
  @ </table>
}