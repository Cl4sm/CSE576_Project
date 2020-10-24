     SCM num;
     char *pos, *s_caller;
{
  unsigned long res;
  if (INUMP(num)) {
    ASRTGO(0 < num, errout);
    res = INUM((unsigned long)num);
    return res;
  }
  ASRTGO(NIMP(num), errout);
#ifdef FLOATS
  if (REALP(num)) {
    double u = REALPART(num);
    if ((0 <= u) && (u <= (unsigned long)~0L)) {
      res = u;
      return res;
    }
  }
#endif
#ifdef BIGDIG
  if (TYP16(num)==tc16_bigpos) {
    sizet l = NUMDIGS(num);
    ASRTGO(DIGSPERLONG >= l, errout);
    res = 0;
    for (;l--;) res = BIGUP(res) + BDIGITS(num)[l];
    return res;
  }
#endif
 errout: wta(num, pos, s_caller);
}
