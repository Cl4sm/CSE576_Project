void cgi_checkbox_fieldset(
  const char *zTitle,     /* Title of <fieldset> */
  const char *zClass,     /* CSS class to apply to fieldset */
  int *nTabIndex,         /* This will be incremented for each element. 
                          ** Calling func should be able to continue with it */
  ...                     /* NULL-terminated list of
                          ** name/value/accesskey/label trios */
){
  va_list ap;
  int nAccessKey;
  char *zName, *zVal, *zDflt, *zLabel;
  
  if( zTitle && zTitle[0] ){
    @ <fieldset\
    if( zClass && zClass[0] ){
      @  class="%h(zClass)"\
    }
    @ ><legend>%s(zTitle)</legend>
  }
  va_start(ap, nTabIndex);
  while( (zName = va_arg(ap, char*))!=0 ){
    zVal = va_arg(ap, char*);
    nAccessKey = va_arg(ap, int);
    zDflt = va_arg(ap, char*);
    zLabel = va_arg(ap, char*);
    if( !zName || !zName[0] || !zVal || !zVal[0] || !zLabel || !zLabel[0] ){
      break;
    }
    /* Label won't work without id in IE so we dummy up id here.
    ** Hopefully this won't clash with anyones's CSS.
    */
    cgi_checkbox(zName, 0, 0, nAccessKey, nTabIndex==0 ? 0 : ++(*nTabIndex), 0,
              zVal, zDflt, zLabel);
  }
  va_end(ap);
  if( nTabIndex ) (*nTabIndex)++;
  if( zTitle && zTitle[0] ){
    @ </fieldset>
  }
}