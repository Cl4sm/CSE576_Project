time_t parse_time(const char *zTime){
  char zMonth[30];
  struct tm sTm;
  int y, d, h, m, s;
  int i, n;
  static struct {
    char *zName;
    int iMon;
  } aMName[] = {
    { "january",    1 },
    { "february",   2 },
    { "march",      3 },
    { "april",      4 },
    { "may",        5 },
    { "june",       6 },
    { "july",       7 },
    { "jly",        7 },
    { "august",     8 },
    { "sepember",   9 },
    { "october",   10 },
    { "november",  11 },
    { "december",  12 },
  };

  y = d = h = m = s = 0;
  n = sscanf(zTime, "%d-%20[^-]-%d %d:%d:%d", &y, zMonth, &d, &h, &m, &s);
  if( n<3 || n==4 ) return 0;
  for(i=0; zMonth[i]; i++){
    if( isupper(zMonth[i]) ) zMonth[i] = tolower(zMonth[i]);
  }
  n = strlen(zMonth);
  memset(&sTm, 0, sizeof(sTm));
  if( n<=2 && isdigit(zMonth[0]) && (n==1 || isdigit(zMonth[1]))
      && (sTm.tm_mon = atoi(zMonth))>=1 && sTm.tm_mon<=12 ){
    sTm.tm_mon--;
  }else{
    for(i=0; i<sizeof(aMName)/sizeof(aMName[0]); i++){
      if( strncmp(zMonth, aMName[i].zName, n)==0 ){
        sTm.tm_mon = aMName[i].iMon - 1;
        break;
      }
    }
#if CVSTRAC_I18N
    if( i>=sizeof(aMName)/sizeof(aMName[0]) ){
      for(i=0; i<12; i++){
        if( strncasecmp(zMonth, nl_langinfo(ABMON_1+i), n)==0
                 || strncasecmp(zMonth, nl_langinfo(MON_1+i),n )==0 ){
          sTm.tm_mon = i;
          break;
        }
      }
    }
#endif
  }
  sTm.tm_year = y - 1900;
  sTm.tm_mday = d;
  sTm.tm_hour = h;
  sTm.tm_min = m;
  sTm.tm_sec = s;
  sTm.tm_isdst = -1;
  return mktime(&sTm);
}