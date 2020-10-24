tm_format_datetime(char *x, struct timeformat *fmt_spec, bird_clock_t t)
{
  const char *fmt_used;
  struct tm *tm;
  bird_clock_t delta = now - t;
  t = now_real - delta;
  tm = localtime(&t);

  if (fmt_spec->fmt1 == NULL)
    return tm_format_reltime(x, tm, delta);

  if ((fmt_spec->limit == 0) || (delta < fmt_spec->limit))
    fmt_used = fmt_spec->fmt1;
  else
    fmt_used = fmt_spec->fmt2;

  int rv = strftime(x, TM_DATETIME_BUFFER_SIZE, fmt_used, tm);
  if (((rv == 0) && fmt_used[0]) || (rv == TM_DATETIME_BUFFER_SIZE))
    strcpy(x, "<too-long>");
}
