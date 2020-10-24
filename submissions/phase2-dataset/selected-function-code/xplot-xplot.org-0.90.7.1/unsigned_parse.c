coord unsigned_parse(char *s)
{
  coord r;
#ifndef LIBC_ATOI_IS_BROKEN
  extern int atoi();

  r.u = atoi(s);
#else
  char *p;
  r.u = 0;
  p = s;

  while (isdigit(*p)) {
    r.u *= 10;
    r.u += (*p - '0');
    p++;
  }
  if (*p != '\0')
    fprintf(stderr,"warning: unsigned_parse format error in string: %s\n", s);
#endif  

  return r;
}