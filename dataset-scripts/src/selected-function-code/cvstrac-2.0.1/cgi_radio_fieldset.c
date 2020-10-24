void cgi_radio_fieldset(
  const char *zTitle,     /* Title of <fieldset>. If 0 <fieldset> is not used */
  const char *zName,      /* CGI param name */
  const char *zClass,     /* CSS class to apply to fieldset */
  int *nTabIndex,         /* This will be incremented for each element. 
                          ** Calling func should be able to continue with it */
  const char *zDflt,      /* Element with this value will be checked */
  ...                     /* NULL-terminated list of value/accesskey/label trios */
){
  char *zVal, *zLabel;
  char nAccessKey;
  va_list ap;
  if( !zName || !zName[0] ) return;
  
  if( zTitle && zTitle[0] ){
    @ <fieldset\
    if( zClass && zClass[0] ){
      @  class="%h(zClass)"\
    }
    @ ><legend>%s(zTitle)</legend>
  }

  /*
  ** args are: accesskey, value, label
  */
  va_start(ap, zDflt);
  while( (zVal = va_arg(ap, char*))!=0 ){
    nAccessKey = va_arg(ap, int);
    zLabel = va_arg(ap, char*);
    if( zLabel==0 ) break;

    cgi_radio(zName, 0, 0, isprint(nAccessKey)? nAccessKey : 0,
              nTabIndex==0 ? 0 : ++(*nTabIndex), 0,
              zVal, zDflt, zLabel);
  }
  va_end(ap);
  if( nTabIndex ) (*nTabIndex)++;
  if( zTitle && zTitle[0] ){
    @ </fieldset>
  }
}