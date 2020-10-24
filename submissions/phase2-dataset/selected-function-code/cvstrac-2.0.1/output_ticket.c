void output_ticket(int tn, int rn){
  if( g.okRead ){
    char *zLink = (rn>0) ? format_link("tktview?tn=%d,%d",tn,rn)
                         : format_link("tktview?tn=%d",tn);
    if( g.okTicketLink ) {
      char **az = db_query(
        "SELECT title,status FROM ticket WHERE tn=%d", tn);
      if( az && az[0] && az[1] ){
        cgi_printf("<a href=\"%z\" title=\"%h\">",zLink,az[0]);
        /* FIXME: should use a <span> with the ticket status as a class */
        if( az[1][0] == 'n' || az[1][0] == 'a' ){
          cgi_printf("#%d",tn);
        }else{
          cgi_printf("<strike>#%d</strike>",tn);
        }
        cgi_printf("</a>");
        return;
      }
    }
    cgi_printf("<a href=\"%z\">#%d</a>",zLink,tn);
  }else{
    cgi_printf("#%d",tn);
  }
}