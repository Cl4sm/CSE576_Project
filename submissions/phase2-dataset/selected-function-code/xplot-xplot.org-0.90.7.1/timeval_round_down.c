coord timeval_round_down(coord c1, coord c2)
{
  coord r;
  struct tm *tmp;

  tmp = localtime((time_t *) &(c1.t.tv_sec));

#ifdef HAVE_TM_GMTOFF
  c1.t.tv_sec += tmp->tm_gmtoff;
#endif
  
  if (c2.t.tv_sec == 0) {
    r.t.tv_sec = c1.t.tv_sec;
    r.t.tv_usec = (c1.t.tv_usec - (c1.t.tv_usec % c2.t.tv_usec));
  } else {
    r.t.tv_usec = 0;
    r.t.tv_sec = c1.t.tv_sec - (c1.t.tv_sec % c2.t.tv_sec);
  }

#ifdef HAVE_TM_GMTOFF
  r.t.tv_sec -= tmp->tm_gmtoff;
#endif

  timeval_fix(&r);
  return r;
}