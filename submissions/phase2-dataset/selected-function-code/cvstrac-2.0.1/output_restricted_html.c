static void output_restricted_html(const char *zText, int nText){
  int i, j, k;
  for(i=0; i<nText; i++){
    if( zText[i]!='<' ) continue;
    if( i+1<nText ){
      k = 1 + (zText[i+1]=='/');
      for(j=k; i+j<nText && isalnum(zText[i+j]); j++){}
      if( isAllowed(&zText[i+k], j-k)
          && isAllowedAttr(&zText[i+j],nText-(i+j)) ){
        if( g.noFollow && zText[i+j]!='>' && isLinkTag(&zText[i+k],j-k) ){
          /* link tags are special. We want to allow them
          ** but in order to discourage wiki spam we want to insert
          ** something in the attributes... Note that we don't bother
          ** when the tag doesn't have attributes.
          */
          cgi_append_content(zText,i + j);
          zText += i+j;
          nText -= i+j;
          cgi_printf(" rel=\"nofollow\" ");
          i = -1;
        }
        continue;
      }
    }
    cgi_append_content(zText,i);
    cgi_printf("&lt;");
    zText += i+1;
    nText -= i+1;
    i = -1;
  }
  cgi_append_content(zText,i);
}