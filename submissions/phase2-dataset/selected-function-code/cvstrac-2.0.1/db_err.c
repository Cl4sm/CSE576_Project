void db_err(const char *zReason, const char *zQuery, const char *zFormat, ...){
  char *zMsg;

  va_list ap;
  va_start(ap, zFormat);
  zMsg = vmprintf(zFormat, ap);
  va_end(ap);
  cgi_reset_content();
  cgi_set_status(200, "OK");
  cgi_set_content_type("text/html");
  @ <h1>Database Error</h1>
  @ %h(zMsg)
  if( zQuery ){
    @ <blockquote>%h(zQuery)</blockquote>
  }
  if( zReason ){
    @ Reason: %h(zReason)
  }
  cgi_append_header("Pragma: no-cache\r\n");
  cgi_reply();
  db_close();
  exit(0);
}