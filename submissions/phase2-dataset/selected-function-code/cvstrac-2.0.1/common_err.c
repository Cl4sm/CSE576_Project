void common_err(const char *zFormat, ...){
  char *zMsg;

  va_list ap;
  va_start(ap, zFormat);
  zMsg = vmprintf(zFormat, ap);
  va_end(ap);
  cgi_reset_content();
  common_standard_menu(0,0);
  common_header("Oops!");
  @ <p>The following error has occurred:</p>
  @ <blockquote>%h(zMsg)</blockquote>
  if( g.okSetup ){
    @ <p>Query parameters:<p>
    cgi_print_all();
  }
  common_footer();
  cgi_append_header("Pragma: no-cache\r\n");
  cgi_reply();
  exit(0);
}