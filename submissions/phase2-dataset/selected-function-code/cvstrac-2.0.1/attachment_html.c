int attachment_html(const char *zPage, const char *zBefore, const char *zAfter){
  char **az;
  int i = 0;
  time_t now;
  if( is_integer(zPage) ){
    if( !g.okRead ) return 0;
  }else{
    if( !g.okRdWiki ) return 0;
  }
  az = db_query("SELECT atn, size, date, user, mime, fname, description "
                "FROM attachment WHERE tn='%q' ORDER BY date", zPage);
  time(&now);
  if( az[0] ){
    @ %s(zBefore)
    @ <ul>
    for(i=0; az[i]; i+=7){
      int atn = atoi(az[i]);
      char zDate[200];
      struct tm *pTm;
      time_t t = atoi(az[i+2]);
      pTm = gmtime(&t);
      strftime(zDate, sizeof(zDate), "%Y-%b-%d %H:%M:%S", pTm);
      @ <li><a href="attach_get/%d(atn)/%t(az[i+5])">%h(az[i+5])</a>
      @ %h(az[i+1]) bytes added by %h(az[i+3]) on %h(zDate) UTC.
      if(az[i+6] && az[i+6][0]){
        @ <br>
        output_formatted(az[i+6], NULL);
        @ <br>
      }
      if( ok_to_delete_attachment(t, az[i+3]) ){
        @ [<a href="attach_del?atn=%d(atn)">delete</a>]
      }
    }
    @ </ul>
    @ %s(zAfter)
  }
  db_query_free(az);
  return i/7;
}