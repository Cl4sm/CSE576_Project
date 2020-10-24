void browse_fileview(void){
  const char *zFile = g.zExtra ? g.zExtra : P("f");
  const char *zVers = PD("v","");
  char *zGetFile;
  char *zDir, *z;
  char *zSuffix;
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
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  common_standard_menu(0, "search?f=1");
  history_update(0);

  /* Make sure we always have '/' in zFile, otherwise link to parent
  ** directory won't work for file in repository root.
  */
  if( strrchr(zFile, '/') ){
    zDir = mprintf("%T?d=%T", default_browse_url(), zFile);
  }else{
    zDir = mprintf("%T?d=/%T", default_browse_url(), zFile);
  }
  z = strrchr(zDir, '/' );
  if( z ){ *z = 0;}
  common_add_nav_item(zDir, "Directory");

  zGetFile = mprintf("getfile?f=%T&v=%T", zFile, zVers);
  common_add_action_item(zGetFile, "Raw");
  add_file_tools(0,zFile,zVers,0);

  common_add_help_item("CvstracFileview");
  common_header("%h %h", zFile, printable_vers(zVers));

  /* sort out the MIME type. We output HTML, but some things are embeddable. */
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

  @ <a href="rlog?f=%T(zFile)">%h(zFile)</a>
  @ <a href="%s(zGetFile)">%h(zVers)</a><hr>

  /* For image types, embed in the page. Anything else, try to inline */
  if( !strncmp(zMime,"image/",6) ){
    @ <img src="%s(zGetFile)" alt="%h(zFile) %h(zVers)">
  }else{
    if( dump_version(zVers,zFile,0) ){
      cgi_redirect("index");
      return;
    }
  }

  @ <hr>
  common_footer();
}