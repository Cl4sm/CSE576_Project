void wiki_text_page(void){
  const char *pg = P("p");
  const char *zTime = P("t");
  int tm;
  char **azPage;

  login_check_credentials();
  if( !g.okRdWiki ){ login_needed(); return; }
  throttle(0,0);
  if( zTime==0 || (tm = atoi(zTime))==0 ){
    time_t now;
    time(&now);
    tm = now;
  }
  if( pg==0 || is_wiki_name(pg)!=strlen(pg) ){
    pg = "WikiIndex";
  }
  azPage = db_query(
    "SELECT text,invtime FROM wiki WHERE name='%q' AND invtime>=%d LIMIT 1", pg, -tm
  );

  if( tm == -atoi(azPage[1]) ){
    /* Specific versions of wiki text never change... However, the match was
    ** maybe a bit fuzzy so we only do this stuff if there was a specific
    ** timestamp specified that actually matches the page timestamp.
    */
    cgi_modified_since(tm);
    cgi_append_header(mprintf("Last-Modified: %h\r\n",
                              cgi_rfc822_datestamp(tm)));
  }

  cgi_set_content_type("text/plain");
  cgi_append_content(azPage[0],strlen(azPage[0]));
}