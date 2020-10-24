const char *
date(time_t lclock)
{
  static char buf[80], plus;
  struct tm *lt, *gm;
  struct tm gmbuf;
  int minswest;

  if (!lclock)
    lclock = CurrentTime;

  gm = gmtime(&lclock);
  memcpy(&gmbuf, gm, sizeof(gmbuf));
  gm = &gmbuf;
  lt = localtime(&lclock);

  /*
   * There is unfortunately no clean portable way to extract time zone
   * offset information, so do ugly things.
   */
  minswest = (gm->tm_hour - lt->tm_hour) * 60 + (gm->tm_min - lt->tm_min);

  if (lt->tm_yday != gm->tm_yday)
  {
    if ((lt->tm_yday > gm->tm_yday && lt->tm_year == gm->tm_year) ||
        (lt->tm_yday < gm->tm_yday && lt->tm_year != gm->tm_year))
      minswest -= 24 * 60;
    else
      minswest += 24 * 60;
  }

  plus = (minswest > 0) ? '-' : '+';
  if (minswest < 0)
    minswest = -minswest;

  snprintf(buf, sizeof(buf), "%s %s %d %d -- %02u:%02u:%02u %c%02u:%02u",
           weekdays[lt->tm_wday], months[lt->tm_mon],lt->tm_mday,
           lt->tm_year + 1900, lt->tm_hour, lt->tm_min, lt->tm_sec,
           plus, minswest/60, minswest%60);
  return buf;
}
