void cgi_redirect(const char *zURL){
  char *zLocation;
  if( strncmp(zURL,"http:",5)==0 || strncmp(zURL,"https:",6)==0 || *zURL=='/' ){
    /* An absolute URL.  Do nothing */
  }else{
    int i, j, k=0;
    char *zDest;
    char *zCur = getenv("REQUEST_URI");
    if( zCur==0 ) zCur = "";
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
    zDest = mprintf("%.*s/%s", i, zCur, zURL);
    /* don't touch the protocol stuff, if it exists */
    if( !strncmp(zDest,"http://",7) ){
      k = 7;
    }else if( !strncmp(zDest,"https://",8) ){
      k = 8;
    }
    /* strip out constructs like .., /./, //, etc */
    for(i=j=k; zDest[i]; i++){
      if( zDest[i]=='/' ){
        if( zDest[i+1]=='.' ){
          if( zDest[i+2]=='/' ){
            i += 2;
            continue;
          }
          if( zDest[i+2]=='.' && zDest[i+3]=='/' ){
            if( j==0 ){
              i += 3;
              continue;
            }
            j--;
            while( j>0 && zDest[j-1]!='/' ){ j--; }
            continue;
          }
        }
        if( zDest[i+1]=='/' && (i==0 || zDest[i-1]!=':') ) continue;
      }
      zDest[j++] = zDest[i];
    }
    zDest[j] = 0;
    zURL = zDest;

    /* see if we've got a cycle by matching everything up to the ? or #
    ** in the new and old URLs.
    */
    if( is_same_page(zDest,zCur) ){
      cgi_reset_content();
      cgi_printf("<html>\n<p>Cyclic redirection in %s</p>\n</html>\n", zURL);
      cgi_set_status(500, "Internal Server Error");
      cgi_reply();
      exit(0);
    }
  }

/*
** The lynx browser complains if the "http:" prefix is missing
** from a redirect.  But if we use it, we lose the ability to
** run on a secure server using stunnel.
**
** Relative redirects are used by default.  This works with stunnel.
** Lynx sends us a nasty message, but it still works.  So with
** relative redirects everybody works.  With absolute redirects,
** stunnel will not work.  So the default configuration is to go
** with what works for everybody, even if it happens to be technically
** incorrect.
*/
#ifdef ABSOLUTE_REDIRECT
  {
    char *zHost;
    if( strncmp(zURL,"http:",5)!=0 && strncmp(zURL,"https:",6)!=0 
         && (zHost = getenv("HTTP_HOST"))!=0 ){
      char *zMode = getenv("HTTPS");
      if( zMode && strcmp(zMode,"on")==0 ){
        zURL = mprintf("https://%s%s", zHost, zURL);
      }else{
        zURL = mprintf("http://%s%s", zHost, zURL);
      }
    }
  }
#endif
  zLocation = mprintf("Location: %s\r\n", zURL);
  cgi_append_header(zLocation);
  cgi_reset_content();
  cgi_printf("<html>\n<p>Redirect to %h</p>\n</html>\n", zURL);
  cgi_set_status(302, "Moved Temporarily");
  free(zLocation);
  cgi_reply();
  exit(0);
}