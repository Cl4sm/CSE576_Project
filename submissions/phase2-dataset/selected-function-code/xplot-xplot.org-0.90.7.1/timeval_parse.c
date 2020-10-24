coord timeval_parse(char *s)
{
  coord r;
  extern int atoi();

  r.t.tv_usec = 0;

  /* quick, dirty, and unsafe */
  r.t.tv_sec = atoi(s);
  while (isdigit(*s)) s++;

  if (*s == '.') {
    s++;
    r.t.tv_usec = atoi(s);
    
    {
      int len = 0;
      while (isdigit(*s++)) len++;
      while (len < 6) (len++, r.t.tv_usec *= 10);
      while (len > 6) (len--, r.t.tv_usec /= 10);
    }
  }
  
  timeval_fix(&r);
  return r;
}