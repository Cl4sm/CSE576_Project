void cgi_input_elem(
  int nType,              /* 1:submit, 2:reset, 3:button, 4:file, 5:hidden,
                          ** 6:checkbox, 7:radio, 8:password, 9:text */
  const char *zName,      /* CGI param name */
  const char *zId,        /* HTML element id */
  const char *zClass,     /* CSS class to apply */
  char nAccessKey,        /* Access key to assign */
  int nTabIndex,          /* Element's tab index */
  int nSize,              /* Used only for text fields */
  int nMaxLen,            /* Used only for text fields */
  int nLabelOnLeft,       /* If set, put label text left of element */
  const char *zValue,     /* Element's value */
  const char *zDflt,      /* If same as zValue, "select" this element */
  const char *zLabel      /* Label text. No HTML escaping is done on it */
){
  /* Buttons and hidden fields can't have label
  */
  int bHasLabel = ( nType>4 && zLabel && zLabel[0] );

  assert( nType > 0 );
  assert( nType <= 9 );
  if( zValue==0 ) return;
  if( nType<1 && nType>3 && (!zName || !zName[0]) ) return;
  if( bHasLabel ){
    /* Make sure we have some valid id because <label> won't work in IE w/o it
    */
    if( !zId || !zId[0] ) zId = mprintf("%s%h", zName, zValue);
    @ <label for="%h(zId)">\
    if( nLabelOnLeft>0 ){
      @ %s(zLabel) \
    }
  }
  
  @ <input\
  switch( nType ){
    case 1: cgi_printf(" type=\"submit\""); break;
    case 2: cgi_printf(" type=\"reset\""); break;
    case 3: cgi_printf(" type=\"button\""); break;
    case 4: cgi_printf(" type=\"file\""); break;
    case 5: cgi_printf(" type=\"hidden\""); break;
    case 6: cgi_printf(" type=\"checkbox\""); break;
    case 7: cgi_printf(" type=\"radio\""); break;
    case 8: cgi_printf(" type=\"password\""); break;
    case 9:
      @  type="text"\
      if( nSize>0 ){
        @  size="%d(nSize)"\
      }
      if( nMaxLen>0 ){
        @  maxlength="%d(nMaxLen)"\
      }
      break;
    default: return;
  }
  
  if( zName && zName[0] ){
    @  name="%h(zName)"\
  }
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
  if( zValue && zValue[0] ){
    @  value="%h(zValue)"\
  }
  if( zDflt && zDflt[0] && strcmp(zDflt, zValue)==0 ){
    @  checked\
  }  
  @ >\
  
  if( bHasLabel ){
    if( nLabelOnLeft<=0 ){
      @  %s(zLabel)\
    }
    @ </label>
  }else{
    @ 
  }
}