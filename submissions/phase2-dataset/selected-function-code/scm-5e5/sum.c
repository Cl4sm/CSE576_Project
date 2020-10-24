     SCM x, y;
{
  if (UNBNDP(y)) {
    if (UNBNDP(x)) return INUM0;
#ifndef RECKLESS
    if (!(NUMBERP(x)))
      badx: wta(x, (char *)ARG1, s_sum);
#endif
    return x;
  }
#ifdef FLOATS
  if (NINUMP(x)) {
    SCM t;
# ifdef BIGDIG
    ASRTGO(NIMP(x), badx);
    if (BIGP(x)) {
      if (INUMP(y)) {t = x; x = y; y = t; goto intbig;}
      ASRTGO(NIMP(y), bady);
      if (BIGP(y)) {
	if (NUMDIGS(x) > NUMDIGS(y)) {t = x; x = y; y = t;}
	return addbig(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y, 0);
      }
      ASRTGO(INEXP(y), bady);
    bigreal: return makdbl(big2dbl(x)+REALPART(y), CPLXP(y)?IMAG(y):0.0);
    }
    ASRTGO(INEXP(x), badx);
# else
    ASRTGO(NIMP(x) && INEXP(x), badx);
# endif
    if (INUMP(y)) {t = x; x = y; y = t; goto intreal;}
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) {t = x; x = y; y = t; goto bigreal;}
#  ifndef RECKLESS
    else if (!(INEXP(y)))
      bady: wta(y, (char *)ARG2, s_sum);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && INEXP(y)))
      bady: wta(y, (char *)ARG2, s_sum);
#  endif
# endif
    {
      double i = 0.0;
      if (CPLXP(x)) i = IMAG(x);
      if (CPLXP(y)) i += IMAG(y);
      return makdbl(REALPART(x)+REALPART(y), i);
    }
  }
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y))
      intbig: {
#  ifndef DIGSTOOBIG
      long z = pseudolong(INUM(x));
      return addbig((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0);
#  else
      BIGDIG zdigs[DIGSPERLONG];
      longdigs(INUM(x), zdigs);
      return addbig(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0);
#  endif
    }
    ASRTGO(INEXP(y), bady);
# else
    ASRTGO(NIMP(y) && INEXP(y), bady);
# endif
  intreal: return makdbl(INUM(x)+REALPART(y), CPLXP(y)?IMAG(y):0.0);
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    SCM t;
    ASRTGO(NIMP(x) && BIGP(x), badx);
    if (INUMP(y)) {t = x; x = y; y = t; goto intbig;}
    ASRTGO(NIMP(y) && BIGP(y), bady);
    if (NUMDIGS(x) > NUMDIGS(y)) {t = x; x = y; y = t;}
    return addbig(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y, 0);
  }
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_sum);
#  endif
  intbig: {
#  ifndef DIGSTOOBIG
      long z = pseudolong(INUM(x));
      return addbig((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0);
#  else
      BIGDIG zdigs[DIGSPERLONG];
      longdigs(INUM(x), zdigs);
      return addbig(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0);
#  endif
    }
  }
# else
  ASRTGO(INUMP(x), badx);
  ASRTER(INUMP(y), y, ARG2, s_sum);
# endif
#endif
  x = INUM(x)+INUM(y);
  if (FIXABLE(x)) return MAKINUM(x);
#ifdef BIGDIG
  return long2big(x);
#else
# ifdef FLOATS
  return makdbl((double)x, 0.0);
# else
  wta(y, (char *)OVFLOW, s_sum);
# endif
#endif
}
