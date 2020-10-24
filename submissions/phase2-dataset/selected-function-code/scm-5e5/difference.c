SCM difference(x, y)
     SCM x, y;
{
#ifdef FLOATS
  if (NINUMP(x)) {
# ifndef RECKLESS
    if (!(NIMP(x)))
      badx: wta(x, (char *)ARG1, s_difference);
# endif
    if (UNBNDP(y)) {
# ifdef BIGDIG
      if (BIGP(x)) {
	x = copybig(x, !BIGSIGN(x));
	return NUMDIGS(x) * BITSPERDIG/CHAR_BIT <= sizeof(SCM) ?
	  big2inum(x, NUMDIGS(x)) : x;
      }
# endif
      ASRTGO(INEXP(x), badx);
      return makdbl(-REALPART(x), CPLXP(x)?-IMAG(x):0.0);
    }
    if (INUMP(y)) return sum(x, MAKINUM(-INUM(y)));
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(x)) {
      if (BIGP(y)) return (NUMDIGS(x) < NUMDIGS(y)) ?
		     addbig(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y, 0x0100) :
	addbig(BDIGITS(y), NUMDIGS(y), BIGSIGN(y) ^ 0x0100, x, 0);
      ASRTGO(INEXP(y), bady);
      return makdbl(big2dbl(x)-REALPART(y), CPLXP(y)?-IMAG(y):0.0);
    }
    ASRTGO(INEXP(x), badx);
    if (BIGP(y)) return makdbl(REALPART(x)-big2dbl(y), CPLXP(x)?IMAG(x):0.0);
    ASRTGO(INEXP(y), bady);
# else
    ASRTGO(INEXP(x), badx);
    ASRTGO(NIMP(y) && INEXP(y), bady);
# endif
    if (CPLXP(x)) {
      if (CPLXP(y))
	return makdbl(REAL(x)-REAL(y), IMAG(x)-IMAG(y));
      else
	return makdbl(REAL(x)-REALPART(y), IMAG(x));
    }
    return makdbl(REALPART(x)-REALPART(y), CPLXP(y)?-IMAG(y):0.0);
  }
  if (UNBNDP(y)) {x = -INUM(x); goto checkx;}
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) {
#  ifndef DIGSTOOBIG
      long z = pseudolong(INUM(x));
      return addbig((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0x0100);
#  else
      BIGDIG zdigs[DIGSPERLONG];
      longdigs(INUM(x), zdigs);
      return addbig(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0x0100);
#  endif
    }
#  ifndef RECKLESS
    if (!(INEXP(y)))
      bady: wta(y, (char *)ARG2, s_difference);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && INEXP(y)))
      bady: wta(y, (char *)ARG2, s_difference);
#  endif
# endif
    return makdbl(INUM(x)-REALPART(y), CPLXP(y)?-IMAG(y):0.0);
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_difference);
    if (UNBNDP(y)) {
      x = copybig(x, !BIGSIGN(x));
      return NUMDIGS(x) * BITSPERDIG/CHAR_BIT <= sizeof(SCM) ?
	big2inum(x, NUMDIGS(x)) : x;
    }
    if (INUMP(y)) {
#  ifndef DIGSTOOBIG
      long z = pseudolong(INUM(y));
      return addbig((BIGDIG *)&z, DIGSPERLONG, (y < 0) ? 0 : 0x0100, x, 0);
#  else
      BIGDIG zdigs[DIGSPERLONG];
      longdigs(INUM(x), zdigs);
      return addbig(zdigs, DIGSPERLONG, (y < 0) ? 0 : 0x0100, x, 0);
#  endif
    }
    ASRTGO(NIMP(y) && BIGP(y), bady);
    return (NUMDIGS(x) < NUMDIGS(y)) ?
      addbig(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y, 0x0100) :
      addbig(BDIGITS(y), NUMDIGS(y), BIGSIGN(y) ^ 0x0100, x, 0);
  }
  if (UNBNDP(y)) {x = -INUM(x); goto checkx;}
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_difference);
#  endif
    {
#  ifndef DIGSTOOBIG
      long z = pseudolong(INUM(x));
      return addbig((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0x0100);
#  else
      BIGDIG zdigs[DIGSPERLONG];
      longdigs(INUM(x), zdigs);
      return addbig(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0x0100);
#  endif
    }
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_difference);
  if (UNBNDP(y)) {x = -INUM(x); goto checkx;}
  ASRTER(INUMP(y), y, ARG2, s_difference);
# endif
#endif
  x = INUM(x)-INUM(y);
 checkx:
  if (FIXABLE(x)) return MAKINUM(x);
#ifdef BIGDIG
  return long2big(x);
#else
# ifdef FLOATS
  return makdbl((double)x, 0.0);
# else
  wta(y, (char *)OVFLOW, s_difference);
# endif
#endif
}
