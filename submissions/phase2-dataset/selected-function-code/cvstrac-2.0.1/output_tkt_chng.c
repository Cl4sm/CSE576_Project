static void output_tkt_chng(char **azChng){
  time_t thisDate;
  struct tm *pTm;
  char zDate[100];
  char zPrefix[200];
  char zSuffix[100];
  char *z;
  const char *zType = (atoi(azChng[5])==0) ? "Check-in" : "Milestone";

  thisDate = atoi(azChng[0]);
  pTm = localtime(&thisDate);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M", pTm);
  if( azChng[2][0] ){
    bprintf(zPrefix, sizeof(zPrefix), "%h [%.20h] on branch %.50h: ",
            zType, azChng[1], azChng[2]);
  }else{
    bprintf(zPrefix, sizeof(zPrefix), "%h [%.20h]: ", zType, azChng[1]);
  }
  bprintf(zSuffix, sizeof(zSuffix), " (By %.30h)", azChng[3]);
  @ <tr><td valign="top" width=160 align="right">%h(zDate)</td>
  @ <td valign="top" width=30 align="center">
  common_icon("dot");
  @ </td>
  @ <td valign="top" align="left"> 
  output_formatted(zPrefix, 0);
  z = azChng[4];
  if( output_trim_message(z, MN_CKIN_MSG, MX_CKIN_MSG) ){
    output_formatted(z, 0);
    @ &nbsp;[...]
  }else{
    output_formatted(z, 0);
  }
  output_formatted(zSuffix, 0);
  @ </td></tr>
}