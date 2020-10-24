time_t mkgmtime(struct tm *p){
  time_t t;
  int nDay;
  int isLeapYr;
  /* Days in each month:       31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 */
  static int priorDays[]   = {  0, 31, 59, 90,120,151,181,212,243,273,304,334 };
  if( p->tm_mon<0 ){
    int nYear = (11 - p->tm_mon)/12;
    p->tm_year -= nYear;
    p->tm_mon += nYear*12;
  }else if( p->tm_mon>11 ){
    p->tm_year += p->tm_mon/12;
    p->tm_mon %= 12;
  }
  isLeapYr = p->tm_year%4==0 && (p->tm_year%100!=0 || (p->tm_year+300)%400==0);
  p->tm_yday = priorDays[p->tm_mon] + p->tm_mday - 1;
  if( isLeapYr && p->tm_mon>1 ) p->tm_yday++;
  nDay = (p->tm_year-70)*365 + (p->tm_year-69)/4 -p->tm_year/100 + 
         (p->tm_year+300)/400 + p->tm_yday;
  t = ((nDay*24 + p->tm_hour)*60 + p->tm_min)*60 + p->tm_sec;
  return t;
}