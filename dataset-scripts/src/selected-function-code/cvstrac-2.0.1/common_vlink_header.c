void common_vlink_header(const char *zUrl, const char *zTitle, va_list ap){
  int i = 0;
  int brk;
  const char *zHeader = 0;
  char *zTitleTxt;

  zTitleTxt = vmprintf(zTitle, ap);
  zHeader = db_config("header", HEADER);
  if( zHeader && zHeader[0] ){
    char *z;
    if( zHeader[0]=='/' && (z = common_readfile(zHeader))!=0 ){
      zHeader = z;
    }
    output_with_subst(zHeader, zTitleTxt);
  }else{
    output_with_subst(HEADER, zTitleTxt);
  }
  @ <table width="100%%" cellpadding=2 border=0>
  @ <tr><td bgcolor="%s(BORDER1)" class="border1">
  @ <table width="100%%" border=0 cellpadding=2 cellspacing=0>
  @ <tr bgcolor="%s(BG1)" class="bkgnd1">
  @ <td valign="top" align="left">
  if( zUrl ){
    @ <big><b>%h(g.zName) -
    @ <a rel="nofollow" href="%h(zUrl)">%h(zTitleTxt)</a></b></big><br>
  }else{
    @ <big><b>%h(g.zName) - %h(zTitleTxt)</b></big><br>
  }
  if( !g.isAnon ){
    @ <small><a href="logout" title="Logout %h(g.zUser)">Logged in</a> as
    if( !strcmp(g.zUser,"setup") ){
      @ setup
    }else{
      @ <a href="wiki?p=%T(g.zUser)">%h(g.zUser)</a>
    }
    @ </small>
  }else{
    /* We don't want to be redirected back to captcha page, but ratehr to 
    ** one from which we were redirected to captcha in the first place.
    */
    const char *zUri = (P("cnxp")!=0) ? P("cnxp") : getenv("REQUEST_URI");
    @ <a href="honeypot"><small><notatag arg="meaningless"></small></a>
    @ <small><a href="login?nxp=%T(zUri)" title="Log in">Not logged in</a></small>
  }
  @ </td>
  @ <td valign="bottom" align="right">
  if( nLink ){
    int j;
    int nChar;
    nLink /= 2;
    qsort(azLink, nLink, 2*sizeof(azLink[0]), link_compare);
    nChar = 0;
    for(i=0; azLink[i]; i+=2){
      nChar += strlen(azLink[i+1]) + 3;
    }
    if( nChar<=60 ){
      brk = nChar;
    }else if( nChar<=120 ){
      brk = nChar/2;
    }else{
      brk = nChar/3;
    }
    nChar = 0;
    @ <nobr>
    for(i=0, j=1; azLink[i] && azLink[i+1]; i+=2, j++){
      const char *z = azLink[i+1];
      if( z[0]<'A' ) z++;
      @ [<a href="%h(azLink[i])">%h(z)</a>]&nbsp;
      nChar += strlen(azLink[i+1]) + 3;
      if( nChar>=brk && azLink[i+2] ){
        nChar = 0;
        @ </nobr><br><nobr>
      }
    }
    @ </nobr>
  }
  if( i==0 ){
    @ &nbsp;
  }
  @ </td></tr>
  if( nAction ){
    int j;
    int nChar;

    @ <tr bgcolor="%s(BG4)" class="bkgnd4">
    @ <td>&nbsp;</td>
    @ <td valign="bottom" align="right">

    nAction /= 2;
    qsort(azAction, nAction, 2*sizeof(azAction[0]), link_compare);
    nChar = 0;
    for(i=0; azAction[i]; i+=2){
      nChar += strlen(azAction[i+1]) + 3;
    }
    if( nChar<=60 ){
      brk = nChar;
    }else if( nChar<=120 ){
      brk = nChar/2;
    }else{
      brk = nChar/3;
    }
    nChar = 0;
    @ <nobr>
    for(i=0, j=1; azAction[i] && azAction[i+1]; i+=2, j++){
      const char *z = azAction[i+1];
      if( z[0]<'A' ) z++;
      @ [<a href="%h(azAction[i])" rel="nofollow">%h(z)</a>]&nbsp;
      nChar += strlen(azAction[i+1]) + 3;
      if( nChar>=brk && azAction[i+2] ){
        nChar = 0;
        @ </nobr><br><nobr>
      }
    }
    @ </nobr>
    if( i==0 ){
      @ &nbsp;
    }
    @ </td></tr>
  }
  @ </table>
  @ </td></tr></table>
  @ <div id="body">
  free(zTitleTxt);
}