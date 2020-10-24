void patchset_view(void){
  int cn = atoi(PD("cn","0"));
  char *z;

  login_check_credentials();
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);

  cgi_set_content_type("text/plain");

  z = db_short_query("SELECT date FROM chng WHERE cn=%d",cn);
  if( z ) cgi_modified_since(atoi(z));

  cgi_append_header(mprintf("Content-disposition: attachment; "
        "filename=\"%T_patchset_%d.diff\"\r\n", g.zName, cn));

  /* patchsets are raw output based one what are supposed to be unchanging file
  ** revisions.
  */
  g.isConst = 1;

  if( cn>0 ) diff_chng(cn,1);
}