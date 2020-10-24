static char *get_base_url(void){
  int i;
  char *zHost = getenv("HTTP_HOST");
  char *zMode = getenv("HTTPS");
  char *zCur = getenv("REQUEST_URI");
  if( zCur==0 ) zCur = "/";
  if( zHost==0 ) zHost = "";

  for(i=0; zCur[i] && zCur[i]!='?' && zCur[i]!='#'; i++){}
  if( g.zExtra ){
    /* Skip to start of extra stuff, then pass over any /'s that might
    ** have separated the document root from the extra stuff. This
    ** ensures that the redirection actually redirects the root, not
    ** something deep down at the bottom of a URL.
    */
    i -= strlen(g.zExtra);
    while( i>0 && zCur[i-1]=='/' ){ i--; }
  }
  while( i>0 && zCur[i-1]!='/' ){ i--; }
  while( i>0 && zCur[i-1]=='/' ){ i--; }

  if( zMode && strcmp(zMode,"on")==0 ){
    return mprintf("https://%s%.*s", zHost, i, zCur);
  }
  return mprintf("http://%s%.*s", zHost, i, zCur);
}