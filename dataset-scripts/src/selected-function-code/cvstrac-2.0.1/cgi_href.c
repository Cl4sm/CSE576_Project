void cgi_href(
  const char *zText,      /* Link text */
  const char *zId,        /* HTML element id */
  const char *zClass,     /* CSS class to apply */
  char nAccessKey,        /* Access key to assign */
  int nTabIndex,          /* Element's tab index */
  const char *zTitle,     /* Title to apply to <a> */
  const char *zHref,      /* Link address (vxprintf() format) */
  ...
){
  va_list ap;
  
  if( !zText || !zText[0] || !zHref || !zHref[0] ) return;
  
  @ <a href="\
  va_start(ap, zHref);
  cgi_vprintf(zHref, ap);
  va_end(ap);
  @ "\
  
  if( zId && zId[0] ){
    @  id="%h(zId)"\
  }
  if( zClass && zClass[0] ){
    @  class="%h(zClass)"\
  }
  if( isalnum(nAccessKey) ){
    @  accesskey="%c(nAccessKey)"\
  }
  if( nTabIndex>0 ){
    @  tabindex="%d(nTabIndex)"\
  }
  if( zTitle && zTitle[0] ){
    @  title="%h(zTitle)"\
  }
  
  @ >%h(zText)</a>
}