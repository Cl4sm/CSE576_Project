time_t cgi_rfc822_parsedate(const char *zDate){
  static char *azMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                             "Aug", "Sep", "Oct", "Nov", "Dec", 0};
  struct tm t;
  char zIgnore[16];
  char zMonth[16];

  memset(&t, 0, sizeof(t));
  if( 7==sscanf(zDate, "%12[A-Za-z,] %d %12[A-Za-z] %d %d:%d:%d", zIgnore,
                       &t.tm_mday, zMonth, &t.tm_year, &t.tm_hour, &t.tm_min,
                       &t.tm_sec)){

    if( t.tm_year > 1900 ) t.tm_year -= 1900;
    for(t.tm_mon=0; azMonths[t.tm_mon]; t.tm_mon++){
      if( !strncasecmp( azMonths[t.tm_mon], zMonth, 3 )){
        return mkgmtime(&t);
      }
    }
  }

  return 0;
}