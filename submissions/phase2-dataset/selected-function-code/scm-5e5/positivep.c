     SCM x;
{
#ifdef FLOATS
  if (NINUMP(x)) {
# ifdef BIGDIG
    ASRTGO(NIMP(x), badx);
    if (BIGP(x)) return TYP16(x)==tc16_bigpos ? BOOL_T : BOOL_F;
#  ifndef RECKLESS
    if (!(REALP(x)))
      badx: wta(x, (char *)ARG1, s_positivep);
#  endif
# else
    ASRTER(NIMP(x) && REALP(x), x, ARG1, s_positivep);
# endif
    return (REALPART(x) > 0.0) ? BOOL_T : BOOL_F;
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_positivep);
    return TYP16(x)==tc16_bigpos ? BOOL_T : BOOL_F;
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_positivep);
# endif
#endif
  return (x > INUM0) ? BOOL_T : BOOL_F;
}
