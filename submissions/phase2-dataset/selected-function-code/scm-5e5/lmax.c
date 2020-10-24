SCM lmax(x, y)
     SCM x, y;
{
#ifdef FLOATS
  SCM t;
  double z;
#endif
  if (UNBNDP(y)) {
#ifndef RECKLESS
    if (!(NUMBERP(x)))
      badx: wta(x, (char *)ARG1, s_max);
#endif
    return x;
  }
#ifdef FLOATS
  if (NINUMP(x)) {
# ifdef BIGDIG
    ASRTGO(NIMP(x), badx);
    if (BIGP(x)) {
      if (INUMP(y)) return BIGSIGN(x) ? y : x;
      ASRTGO(NIMP(y), bady);
      if (BIGP(y)) return (1==bigcomp(x, y)) ? y : x;
      ASRTGO(REALP(y), bady);
    big_dbl:
      if (-1 != scm_bigdblcomp(x, REALPART(y))) return y;
      z = big2dbl(x);
      ASRTER(0==scm_bigdblcomp(x, z), x, s_exactprob, s_max);
      return makdbl(z, 0.0);
    }
    ASRTGO(REALP(x), badx);
# else
    ASRTER(NIMP(x) && REALP(x), x, ARG1, s_max);
# endif
    if (INUMP(y)) return (REALPART(x) < (z = INUM(y))) ? makdbl(z, 0.0) : x;
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) {
      t = y; y = x; x = t; goto big_dbl;
    }
    ASRTGO(REALP(y), bady);
# else
    ASRTGO(NIMP(y) && REALP(y), bady);
# endif
    return (REALPART(x) < REALPART(y)) ? y : x;
  }
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) return BIGSIGN(y) ? x : y;
#  ifndef RECKLESS
    if (!(REALP(y)))
      bady: wta(y, (char *)ARG2, s_max);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && REALP(y)))
      bady: wta(y, (char *)ARG2, s_max);
#  endif
# endif
    return ((z = INUM(x)) < REALPART(y)) ? y : makdbl(z, 0.0);
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_max);
    if (INUMP(y)) return BIGSIGN(x) ? y : x;
    ASRTGO(NIMP(y) && BIGP(y), bady);
    return (1==bigcomp(x, y)) ? y : x;
  }
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_max);
#  endif
    return BIGSIGN(y) ? x : y;
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_max);
  ASRTER(INUMP(y), y, ARG2, s_max);
# endif
#endif
  return ((long)x < (long)y) ? y : x;
}
