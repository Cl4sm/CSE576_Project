static char *file_age_to_text(int nModified){
  int nAge, n;
  int nYear  = 31536000; /* Number of seconds in a year */
  int nMonth =  2592000; /* Number of seconds in a month */
  int nWeek  =   604800; /* Number of seconds in a week */
  int nDay   =    86400; /* Number of seconds in a day */
  
  if( nModified<=0 ){
    /* FIXME: some error handling would be nice here */
    return NULL;
  }
  
  nAge = (int)time(0)-nModified;
  if( nAge<0 ){
    /* FIXME: some error handling would be nice here */
    return NULL;
  }
  
  if( (n = nAge/nYear)>1 ){
    return mprintf("%d years", n);
  }else if( (n = nAge/nMonth)>1 ){
    return mprintf("%d months", n);
  }else if( (n = nAge/nWeek)>1 ){
    return mprintf("%d weeks", n);
  }else if( (n = nAge/nDay)>1 ){
    return mprintf("%d days", n);
  }else if( (n = nAge/3600)>1 ){
    return mprintf("%d hours", n);
  }else{
    n = nAge/60;
    if( n<=1 ){
      return mprintf("1 minute");
    }else{
      return mprintf("%d minutes", n);
    }
  }
}