SCM divide(x, y)
     SCM x, y;
{
#ifdef FLOATS
  double den, a = 1.0;
  if (NINUMP(x)) {
# ifndef RECKLESS
    if (!(NIMP(x)))
      badx: wta(x, (char *)ARG1, s_divide);
# endif
    if (UNBNDP(y)) {
# ifdef BIGDIG
      if (BIGP(x)) return makdbl(1.0/big2dbl(x), 0.0);
# endif
      /* reciprocal */
      ASRTGO(INEXP(x), badx);
      if (REALP(x)) return makdbl(1.0/REALPART(x), 0.0);
      {
	y = x;
	a = 1.0;
	goto real_over_complex;
      }
    }
# ifdef BIGDIG
    if (BIGP(x)) {
      SCM z;
      if (INUMP(y)) {
        z = INUM(y);
        ASRTER(z, y, OVFLOW, s_divide);
	if (1==z) return x;
        if (z < 0) z = -z;
        if (z < BIGRAD) {
          SCM w = copybig(x, BIGSIGN(x) ? (y>0) : (y<0));
          return divbigdig(BDIGITS(w), NUMDIGS(w), (BIGDIG)z) ?
	    bigdblop('/', x, INUM(y), 0.0) : normbig(w);
	}
#  ifndef DIGSTOOBIG
        z = pseudolong(z);
        z = divbigbig(BDIGITS(x), NUMDIGS(x), (BIGDIG *)&z, DIGSPERLONG,
                      BIGSIGN(x) ? (y>0) : (y<0), 3);
#  else
	{
	  BIGDIG zdigs[DIGSPERLONG];
	  longdigs(z, zdigs);
	  z = divbigbig(BDIGITS(x), NUMDIGS(x), zdigs, DIGSPERLONG,
			BIGSIGN(x) ? (y>0) : (y<0), 3);
	}
#  endif
        return z ? z : bigdblop('/', x, INUM(y), 0.0);
      }
      ASRTGO(NIMP(y), bady);
      if (BIGP(y)) {
	z = divbigbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
		      BIGSIGN(x) ^ BIGSIGN(y), 3);
	return z ? z : inex_divbigbig(x, y);
      }
      ASRTGO(INEXP(y), bady);
      return bigdblop('/', x, REALPART(y), CPLXP(y) ? IMAG(y) : 0.0);
    }
# endif
    ASRTGO(INEXP(x), badx);
    if (INUMP(y)) {den = INUM(y); goto basic_div;}
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) return bigdblop('\\', y, REALPART(x), CPLXP(x) ? IMAG(x) : 0.0);
    ASRTGO(INEXP(y), bady);
# else
    ASRTGO(NIMP(y) && INEXP(y), bady);
# endif
    if (REALP(y)) {
      den = REALPART(y);
    basic_div: return makdbl(REALPART(x)/den, CPLXP(x)?IMAG(x)/den:0.0);
    }
    a = REALPART(x);
    if (REALP(x)) goto real_over_complex;
    /* Both x and y are complex */
    /* Use "Smith's formula" to extend dynamic range */
    {
      double b = IMAG(x);
      double c = REALPART(y);
      double d = IMAG(y);
      if ((d > 0 ? d : -d) < (c > 0 ? c : -c)) {
	double r = d/c;
	double i = c + d*r;
	return makdbl((a + b*r)/i, (b - a*r)/i);
      }
      {
	double r = c/d;
	double i = d + c*r;
	return makdbl((b + a*r)/i, (-a + b*r)/i);
      }
    }
  }
  if (UNBNDP(y)) {
    if ((MAKINUM(1L)==x) || (MAKINUM(-1L)==x)) return x;
    return makdbl(1.0/((double)INUM(x)), 0.0);
  }
  if (NINUMP(y)) {
# ifdef BIGDIG
    ASRTGO(NIMP(y), bady);
    if (BIGP(y)) return bigdblop('\\', y, INUM(x), 0.0);
#  ifndef RECKLESS
    if (!(INEXP(y)))
      bady: wta(y, (char *)ARG2, s_divide);
#  endif
# else
#  ifndef RECKLESS
    if (!(NIMP(y) && INEXP(y)))
      bady: wta(y, (char *)ARG2, s_divide);
#  endif
# endif
    if (REALP(y)) return makdbl(INUM(x)/REALPART(y), 0.0);
    a = INUM(x);
  real_over_complex:
    /* Both x and y are complex */
    /* Use "Smith's formula" to extend dynamic range */
    {
      double c = REALPART(y);
      double d = IMAG(y);
      if ((d > 0 ? d : -d) < (c > 0 ? c : -c)) {
	double r = d/c;
	double i = c + d*r;
	return makdbl((a)/i, (- a*r)/i);
      }
      {
	double r = c/d;
	double i = d + c*r;
	return makdbl((a*r)/i, (-a)/i);
      }
    }
  }
#else
# ifdef BIGDIG
  if (NINUMP(x)) {
    SCM z;
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_divide);
    if (UNBNDP(y)) goto ov;
    if (INUMP(y)) {
      z = INUM(y);
      if (!z) goto ov;
      if (1==z) return x;
      if (z < 0) z = -z;
      if (z < BIGRAD) {
        SCM w = copybig(x, BIGSIGN(x) ? (y>0) : (y<0));
        if (divbigdig(BDIGITS(w), NUMDIGS(w), (BIGDIG)z)) goto ov;
        return w;
      }
#  ifndef DIGSTOOBIG
      z = pseudolong(z);
      z = divbigbig(BDIGITS(x), NUMDIGS(x), (BIGDIG *)&z, DIGSPERLONG,
		    BIGSIGN(x) ? (y>0) : (y<0), 3);
#  else
      {
	BIGDIG zdigs[DIGSPERLONG];
	longdigs(z, zdigs);
	z = divbigbig(BDIGITS(x), NUMDIGS(x), zdigs, DIGSPERLONG,
		      BIGSIGN(x) ? (y>0) : (y<0), 3);
      }
#  endif
    } else {
      ASRTGO(NIMP(y) && BIGP(y), bady);
      z = divbigbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
		    BIGSIGN(x) ^ BIGSIGN(y), 3);
    }
    if (!z) goto ov;
    return z;
  }
  if (UNBNDP(y)) {
    if ((MAKINUM(1L)==x) || (MAKINUM(-1L)==x)) return x;
    goto ov;
  }
  if (NINUMP(y)) {
#  ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
      bady: wta(y, (char *)ARG2, s_divide);
#  endif
    goto ov;
  }
# else
  ASRTER(INUMP(x), x, ARG1, s_divide);
  if (UNBNDP(y)) {
    if ((MAKINUM(1L)==x) || (MAKINUM(-1L)==x)) return x;
    goto ov;
  }
  ASRTER(INUMP(y), y, ARG2, s_divide);
# endif
#endif
  {
    long z = INUM(y);
    if ((0==z) || INUM(x)%z) goto ov;
    z = INUM(x)/z;
    if (FIXABLE(z)) return MAKINUM(z);
#ifdef BIGDIG
    return long2big(z);
#endif
#ifdef FLOATS
  ov: return makdbl(((double)INUM(x))/((double)INUM(y)), 0.0);
#else
  ov: wta(x, (char *)OVFLOW, s_divide);
#endif
  }
}
