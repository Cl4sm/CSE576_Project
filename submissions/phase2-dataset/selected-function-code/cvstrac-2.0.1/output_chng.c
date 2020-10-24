void output_chng(int cn){
  if( g.okRead ){
    char *zLink = format_link("chngview?cn=%d",cn);
    if( g.okCheckinLink ){
      char **az = db_query(
           "SELECT milestone,user,message,branch FROM chng WHERE cn=%d", cn);
      if( az && az[0] && az[1] && az[2] ){
        if( az[0][0] && az[0][0] != '0' ){
          cgi_printf("<a href=\"%z\"",zLink);
          cgi_printf("title=\"Milestone [%d] %h (By %h)\">",
                     cn, az[2], az[1]);
          cgi_printf("[%d]</a>", cn);
        }else{
          char *z = az[2];
          int trimmed;

          /* Mozilla and Firefox are quite sensitive to newlines
          ** in link titles so we can't use '@' formatting here.
          */
          cgi_printf("<a href=\"%z\" title=\"Check-in [%d]", zLink,cn);
          if( az[3] && az[3][0] ){
            cgi_printf("on branch %h", az[3]);
          }
          cgi_printf(":");  /* want this tight to last text */
          trimmed = output_trim_message(z, MN_CKIN_MSG, MX_CKIN_MSG);
          cgi_printf("%h%s (By %h)\">[%d]</a>", z, trimmed?"...":"",
                     az[1],cn);
        }
        return;
      }
    }
    cgi_printf("<a href=\"%z\">[%d]</a>", zLink, cn);
  }else{
    cgi_printf("[%d]", cn);
  }
}