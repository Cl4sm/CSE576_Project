void browse_getfile(void){
  const char *zFile = g.zExtra ? g.zExtra : P("f");
  const char *zVers = P("v");
  char *zSuffix;
  const char *zName;
  char *zMime = "text/plain";  /* The default MIME type */

  /* The following table lists some alternative MIME types based on
  ** the file suffix
  */
  static const struct {
    char *zSuffix;
    char *zMime;
  } aMime[] = {
    { "html",  "text/html" },
    { "htm",   "text/html" },
    { "gif",   "image/gif" },
    { "jpeg",  "image/jpeg" },
    { "jpg",   "image/jpeg" },
    { "png",   "image/png" },
    { "pdf",   "application/pdf" },
    { "ps",    "application/postscript" },
    { "eps",   "application/postscript" },
  };

  login_check_credentials();
  if( !g.okCheckout || zFile==0 ){ login_needed(); return; }
  throttle(1,0);

  if( zVers!= 0 ){
    /* A database query is almost definitely going to be faster than having
    ** to pull from from the repository, so we might as well try this first.
    */
    char *z = db_short_query("SELECT chng.date FROM filechng, chng "
                             "WHERE filechng.filename='%q' "
                             "      AND filechng.vers='%q' "
                             "      AND filechng.cn=chng.cn ",
                             zFile, zVers);
    if( z ){
      cgi_modified_since(atoi(z));
      cgi_append_header(mprintf("Last-Modified: %h\r\n",
                        cgi_rfc822_datestamp(atoi(z))));
      free(z);
    }
  }

  if( dump_version(zVers,zFile,1) ){
    cgi_redirect("index");
    return;
  }

  /* sort out the MIME type */
  zSuffix = strrchr(zFile, '.');
  if( zSuffix ){
    char zLine[2000];
    int i;
    zSuffix++;
    for(i=0; zSuffix[i] && i<sizeof(zLine)-1; i++){
      zLine[i] = tolower(zSuffix[i]);
    }
    zLine[i] = 0;
    for(i=0; i<sizeof(aMime)/sizeof(aMime[0]); i++){
      if( strcmp(zLine, aMime[i].zSuffix)==0 ){
        zMime = aMime[i].zMime;
        break;
      }
    }
  }

  /*
  ** This means the user gets something meaningful as a default filename
  ** when they try to save to file (depending on the browser).
  */
  zName = strrchr(zFile, '/');
  if (zName) zName += 1;
  cgi_append_header(mprintf("Content-disposition: attachment; "
        "filename=\"%T\"\r\n", zName ? zName : zFile));

  if( zVers && zVers[0] ){
    g.isConst = 1;
  }
  cgi_set_content_type(zMime);
  cgi_set_status(200, "OK");
  return;
}