SCM lessp(x, y)
     SCM x, y;
{
#ifdef FLOATS
  if (NINUMP(x)) {
# ifdef BIGDIG
#  ifndef RECKLESS
    if (!(NIMP(x)))
      badx: wta(x, (char *)ARG1, s_lessp);
#  endif
    if (BIGP(x)) {
      if (INUMP(y)) return BIGSIGN(x) ? BOOL_T : BOOL_F;
      ASRTGO(NIMP(y), bady);
      if (BIGP(y)) return (1==bigcomp(x, y)) ? BOOL_T : BOOL_F;
      ASRTGO(REALP(y), bady);
      return (1==scm_bigdblcomp(x, REALPART(y))) ? BOOL_T : BOOL_F;
    }
    ASRTGO(REALP(x), badx);
# else
    ASRTER(NIMP(x) && REALP(x), x, ARG1, s_lessp);
# endif
    if (INUMP(y)) return (REALPART(x) < ((double)INUM(y))) ? BOOL_T : BOOL_F;
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) return (-1==scm_bigdblcomp(y, REALPART(x))) ? BOOL_T : BOOL_F;
    ASRTGO(REALP(y), bady);
# else
    ASRTGO(NIMP(y) && REALP(y), bady);
# endif
    return (REALPART(x) < REALPART(y)) ? BOOL_T : BOOL_F;
  }
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) return BIGSIGN(y) ? BOOL_F : BOOL_T;
#  ifndef RECKLESS
    if (!(REALP(y)))
      bady: wta(y, (char *)ARG2, s_lessp);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && REALP(y)))
      bady: wta(y, (char *)ARG2, s_lessp);
#  endif
# endif
    return (((double)INUM(x)) < REALPART(y)) ? BOOL_T : BOOL_F;
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_lessp);
    if (INUMP(y)) return BIGSIGN(x) ? BOOL_T : BOOL_F;
    ASRTGO(NIMP(y) && BIGP(y), bady);
    return (1==bigcomp(x, y)) ? BOOL_T : BOOL_F;
  }
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_lessp);
#  endif
    return BIGSIGN(y) ? BOOL_F : BOOL_T;
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_lessp);
  ASRTER(INUMP(y), y, ARG2, s_lessp);
# endif
#endif
  return ((long)x < (long)y) ? BOOL_T : BOOL_F;
}
