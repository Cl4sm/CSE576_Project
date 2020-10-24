static void ticket_checkin(
  time_t date,          /* date/time of the change */
  int cn,               /* change number */
  const char *zBranch,  /* branch of the change, may be NULL */
  const char *zUser,    /* user name that made the change */
  const char *zMessage  /* log message for the change */
){
  struct tm *pTm;
  char *z;
  char zDate[100];

  @ <li> Check-in 

  output_chng(cn);
  if( zBranch && zBranch[0] ){
    @ on branch %h(zBranch):
  } else {
    cgi_printf(": "); /* want the : right up against the [cn] */
  }

  z = strdup(zMessage);
  if( output_trim_message(z, MN_CKIN_MSG, MX_CKIN_MSG) ){
    output_formatted(z, 0);
    @ &nbsp;[...]
  }else{
    output_formatted(z, 0);
  }

  pTm = localtime(&date);
  strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M", pTm);
  @ (By %h(zUser) on %h(zDate))</li>
  @ </li>
}