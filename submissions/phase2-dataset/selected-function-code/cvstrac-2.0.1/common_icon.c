void common_icon(const char* zIcon){
  /* use HTML 4.0 character entities to create symbolic "icons". However,
  ** it seems that some browsers (Konqueror, maybe Safari) don't support
  ** the full set of entities. Some compromises are made.
  **
  ** These would all fit very nicely into the config table. Then this
  ** function could be reduced to just:
  **   z=db_config(zIcon,"&bull;");
  **   @ %s(z)
  */
  if( !strcmp(zIcon,"arrow") ){
    cgi_printf("<font color=\"blue\">&rsaquo;</font>");
  }else if( !strcmp(zIcon,"box") ){
    cgi_printf("<font color=\"#007878\">&curren;</font>");
  }else if( !strcmp(zIcon,"ck") ){
    cgi_printf("<font color=\"green\">&radic;</font>");
  }else if( !strcmp(zIcon,"dia") ){
    cgi_printf("<font color=\"orange\">&diams;</font>");
  }else if( !strcmp(zIcon,"dot") ){
    cgi_printf("<font color=\"blue\">&bull;</font>");
  }else if( !strcmp(zIcon,"ptr1") ){
    cgi_printf("<font color=\"purple\">&raquo;</font>");
  }else if( !strcmp(zIcon,"star") ){
    cgi_printf("<font color=\"#8C80A300\">&#42;</font>");
  }else if( !strcmp(zIcon,"x") ){
    cgi_printf("<font color=\"red\">&times;</font>");
  }else if( !strcmp(zIcon,"del") ){
    cgi_printf("<font color=\"red\">&times;</font>");
  }else if( !strcmp(zIcon,"file") ){
    cgi_printf("<font color=\"black\">&bull;</font>");
  }else if( !strcmp(zIcon,"dir") ){
    cgi_printf("<font color=\"green\">&raquo</font>");
  }else if( !strcmp(zIcon,"backup") ){
    cgi_printf("<font color=\"black\">&laquo;</font>");
  }
}