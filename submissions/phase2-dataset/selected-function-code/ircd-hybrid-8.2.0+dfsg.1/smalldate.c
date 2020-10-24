const char *
smalldate(time_t lclock)
{
  static char buf[MAX_DATE_STRING];
  struct tm *lt, *gm;
  struct tm gmbuf;

  if (!lclock)
    lclock = CurrentTime;

  gm = gmtime(&lclock);
  memcpy(&gmbuf, gm, sizeof(gmbuf));
  gm = &gmbuf;
  lt = localtime(&lclock);

  snprintf(buf, sizeof(buf), "%d/%d/%d %02d.%02d",
           lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
           lt->tm_hour, lt->tm_min);

  return buf;
}
