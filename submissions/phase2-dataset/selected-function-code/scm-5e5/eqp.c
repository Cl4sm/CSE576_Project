     SCM x, y;
{
#ifdef FLOATS
  SCM t;
  if (NINUMP(x)) {
# ifdef BIGDIG
#  ifndef RECKLESS
    if (!(NIMP(x)))
      badx: wta(x, (char *)ARG1, s_eqp);
#  endif
    if (BIGP(x)) {
      if (INUMP(y)) return BOOL_F;
      ASRTGO(NIMP(y), bady);
      if (BIGP(y)) return (0==bigcomp(x, y)) ? BOOL_T : BOOL_F;
      ASRTGO(INEXP(y), bady);
    bigreal:
      return (REALP(y) && (0==scm_bigdblcomp(x, REALPART(y)))) ?
	BOOL_T : BOOL_F;
    }
    ASRTGO(INEXP(x), badx);
# else
    ASRTER(NIMP(x) && INEXP(x), x, ARG1, s_eqp);
# endif
    if (INUMP(y)) {t = x; x = y; y = t; goto realint;}
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) {t = x; x = y; y = t; goto bigreal;}
    ASRTGO(INEXP(y), bady);
# else
    ASRTGO(NIMP(y) && INEXP(y), bady);
# endif
    if (x==y) return BOOL_T;
    return floequal(x, y);
  }
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) return BOOL_F;
#  ifndef RECKLESS
    if (!(INEXP(y)))
      bady: wta(y, (char *)ARG2, s_eqp);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && INEXP(y)))
      bady: wta(y, (char *)ARG2, s_eqp);
#  endif
# endif
  realint:
    return (REALP(y) && (((double)INUM(x))==REALPART(y))) ? BOOL_T : BOOL_F;
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_eqp);
    if (INUMP(y)) return BOOL_F;
    ASRTGO(NIMP(y) && BIGP(y), bady);
    return (0==bigcomp(x, y)) ? BOOL_T : BOOL_F;
  }
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_eqp);
#  endif
    return BOOL_F;
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_eqp);
  ASRTER(INUMP(y), y, ARG2, s_eqp);
# endif
#endif
  return ((long)x==(long)y) ? BOOL_T : BOOL_F;
}
