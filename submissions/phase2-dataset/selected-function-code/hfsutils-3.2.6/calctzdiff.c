static
void calctzdiff(void)
{
# ifdef HAVE_MKTIME

  time_t t;
  int isdst;
  struct tm tm;
  const struct tm *tmp;

  time(&t);
  isdst = localtime(&t)->tm_isdst;

  tmp = gmtime(&t);
  if (tmp)
    {
      tm = *tmp;
      tm.tm_isdst = isdst;

      tzdiff = t - mktime(&tm);
    }
  else
    tzdiff = 0;

# else

  tzdiff = 0;

# endif
}
