SCM product(x, y)
     SCM x, y;
{
  if (UNBNDP(y)) {
    if (UNBNDP(x)) return MAKINUM(1L);
#ifndef RECKLESS
    if (!(NUMBERP(x)))
      badx: wta(x, (char *)ARG1, s_product);
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
      if (BIGP(y)) return mulbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
				 BIGSIGN(x) ^ BIGSIGN(y));
      ASRTGO(INEXP(y), bady);
    bigreal:
      return bigdblop('*', x, REALPART(y), CPLXP(y) ? IMAG(y) : 0.0);
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
      bady: wta(y, (char *)ARG2, s_product);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && INEXP(y)))
      bady: wta(y, (char *)ARG2, s_product);
#  endif
# endif
    if (CPLXP(x)) {
      if (CPLXP(y))
	return makdbl(REAL(x)*REAL(y)-IMAG(x)*IMAG(y),
		      REAL(x)*IMAG(y)+IMAG(x)*REAL(y));
      else
	return makdbl(REAL(x)*REALPART(y), IMAG(x)*REALPART(y));
    }
    return makdbl(REALPART(x)*REALPART(y),
		  CPLXP(y)?REALPART(x)*IMAG(y):0.0);
  }
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) {
    intbig: if (INUM0==x) return x; if (MAKINUM(1L)==x) return y;
    {
#  ifndef DIGSTOOBIG
      long z = pseudolong(INUM(x));
      return mulbig((BIGDIG *)&z, DIGSPERLONG, BDIGITS(y), NUMDIGS(y),
		    BIGSIGN(y) ? (x>0) : (x<0));
#  else
      BIGDIG zdigs[DIGSPERLONG];
      longdigs(INUM(x), zdigs);
      return mulbig(zdigs, DIGSPERLONG, BDIGITS(y), NUMDIGS(y),
		    BIGSIGN(y) ? (x>0) : (x<0));
#  endif
    }
    }
    ASRTGO(INEXP(y), bady);
# else
    ASRTGO(NIMP(y) && INEXP(y), bady);
# endif
  intreal: return makdbl(INUM(x)*REALPART(y), CPLXP(y)?INUM(x)*IMAG(y):0.0);
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTGO(NIMP(x) && BIGP(x), badx);
    if (INUMP(y)) {SCM t = x; x = y; y = t; goto intbig;}
    ASRTGO(NIMP(y) && BIGP(y), bady);
    return mulbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
		  BIGSIGN(x) ^ BIGSIGN(y));
  }
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_product);
#  endif
  intbig: if (INUM0==x) return x; if (MAKINUM(1L)==x) return y;
  {
#  ifndef DIGSTOOBIG
    long z = pseudolong(INUM(x));
    return mulbig((BIGDIG *)&z, DIGSPERLONG, BDIGITS(y), NUMDIGS(y),
		  BIGSIGN(y) ? (x>0) : (x<0));
#  else
    BIGDIG zdigs[DIGSPERLONG];
    longdigs(INUM(x), zdigs);
    return mulbig(zdigs, DIGSPERLONG, BDIGITS(y), NUMDIGS(y),
		  BIGSIGN(y) ? (x>0) : (x<0));
#  endif
  }
  }
# else
  ASRTGO(INUMP(x), badx);
  ASRTER(INUMP(y), y, ARG2, s_product);
# endif
#endif
  {
    long i, j, k;
    i = INUM(x);
    if (0==i) return x;
    j = INUM(y);
    k = i * j;
    y = MAKINUM(k);
    if (k != INUM(y) || k/i != j)
#ifdef BIGDIG
      {
	int sgn = (i < 0) ^ (j < 0);
# ifndef DIGSTOOBIG
	i = pseudolong(i);
	j = pseudolong(j);
	return mulbig((BIGDIG *)&i, DIGSPERLONG,
		      (BIGDIG *)&j, DIGSPERLONG, sgn);
# else /* DIGSTOOBIG */
	BIGDIG idigs[DIGSPERLONG];
	BIGDIG jdigs[DIGSPERLONG];
	longdigs(i, idigs);
	longdigs(j, jdigs);
	return mulbig(idigs, DIGSPERLONG, jdigs, DIGSPERLONG, sgn);
# endif
      }
#else
# ifdef FLOATS
    return makdbl(((double)i)*((double)j), 0.0);
# else
    wta(y, (char *)OVFLOW, s_product);
# endif
#endif
    return y;
  }
}
