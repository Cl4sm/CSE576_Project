     SCM num;
     char *pos, *s_caller;
{
  long res;
  if (INUMP(num)) {
    res = INUM((long)num);
    return res;
  }
  ASRTGO(NIMP(num), errout);
# ifdef FLOATS
  if (REALP(num)) {
    double u = REALPART(num);
    if (((MOST_NEGATIVE_FIXNUM * 4) <= u)
	&& (u <= (MOST_POSITIVE_FIXNUM * 4 + 3))) {
      res = u;
      return res;
    }
  }
# endif
# ifdef BIGDIG
  if (BIGP(num)) {
    sizet l = NUMDIGS(num);
    ASRTGO(DIGSPERLONG >= l, errout);
    res = 0;
    for (;l--;) res = BIGUP(res) + BDIGITS(num)[l];
    ASRTGO(0<res, errout);
    return (tc16_bigpos==TYP16(num) ? res : -res);
  }
# endif
 errout: wta(num, pos, s_caller);
}
