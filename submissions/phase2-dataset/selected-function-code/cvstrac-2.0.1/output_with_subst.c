static void output_with_subst(const char *zText, const char *zTitle){
  int i;
  while( zText[0] ){
    for(i=0; zText[i] && zText[i]!='%'; i++){}
    if( i>0 ) cgi_append_content(zText, i);
    if( zText[i]==0 ) break;
    switch( zText[i+1] ){
      case 'T':
        zText += i+2;
        cgi_printf("%h", zTitle);
        break;
      case 'N':
        zText += i+2;
        cgi_printf("%h", g.zName);
        break;
      case 'V':
        zText += i+2;
        cgi_printf("%h", "@VERSION@");
        break;
      case 'B':
        zText += i+2;
        cgi_printf("%h", g.zBaseURL);
        break;
      case '%':
        zText += i+2;
        cgi_printf("%%");
        break;
      default:
        zText += i+1;
        cgi_printf("%%");
        break;
    }
  }
}