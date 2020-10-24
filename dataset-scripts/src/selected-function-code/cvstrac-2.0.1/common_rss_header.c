static void common_rss_header(char *zTitle, char *zDescription, int nBuildDate){
  int nTTL = atoi(db_config("rss_ttl", "60"));
  cgi_set_content_type("text/xml");
  g.zLinkURL = g.zBaseURL;  /* formatting for output links... */
#if CVSTRAC_I18N
  @ <?xml version="1.0" encoding="%h(nl_langinfo(CODESET))"?>
#else
  @ <?xml version="1.0" encoding="ISO-8859-1"?>
#endif
  @ <rss version="2.0">
  @ <channel>
  @ <title>%h(g.zName) - %h(zTitle)</title>
  @ <link>%s(g.zBaseURL)/timeline</link>
  @ <description>%h(zDescription)</description>
  @ <language>en</language>
  @ <pubDate>%h(cgi_rfc822_datestamp( time(0) ))</pubDate>
  if( nBuildDate>0 ){
    const char* zBD = cgi_rfc822_datestamp(nBuildDate);
    @ <lastBuildDate>%h(zBD)</lastBuildDate>
    cgi_append_header(mprintf("Last-Modified: %h\r\n",zBD));
  }
  @ <generator>CVSTrac @VERSION@</generator>
  @ <ttl>%d(nTTL)</ttl>
}