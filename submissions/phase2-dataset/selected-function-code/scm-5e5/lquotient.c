SCM lquotient(x, y)
     SCM x, y;
{
  register long z;
#ifdef BIGDIG
  if (NINUMP(x)) {
    long w;
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_quotient);
    if (NINUMP(y)) {
      ASRTGO(NIMP(y) && BIGP(y), bady);
      return divbigbig(BDIGITS(x), NUMDIGS(x), BDIGITS(y), NUMDIGS(y),
		       BIGSIGN(x) ^ BIGSIGN(y), 2);
    }
    z = INUM(y);
    ASRTGO(z, ov);
    if (1==z) return x;
    if (z < 0) z = -z;
    if (z < BIGRAD) {
      w = copybig(x, BIGSIGN(x) ? (y>0) : (y<0));
      divbigdig(BDIGITS(w), NUMDIGS(w), (BIGDIG)z);
      return normbig(w);
    }
# ifndef DIGSTOOBIG
    w = pseudolong(z);
    return divbigbig(BDIGITS(x), NUMDIGS(x), (BIGDIG *)&w, DIGSPERLONG,
		     BIGSIGN(x) ? (y>0) : (y<0), 2);
# else
    { BIGDIG zdigs[DIGSPERLONG];
      longdigs(z, zdigs);
      return divbigbig(BDIGITS(x), NUMDIGS(x), zdigs, DIGSPERLONG,
		       BIGSIGN(x) ? (y>0) : (y<0), 2);
    }
# endif
  }
  if (NINUMP(y)) {
# ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
    bady: wta(y, (char *)ARG2, s_quotient);
# endif
    return INUM0;
  }
#else
  ASRTER(INUMP(x), x, ARG1, s_quotient);
  ASRTER(INUMP(y), y, ARG2, s_quotient);
#endif
  if ((z = INUM(y))==0)
  ov: wta(y, (char *)OVFLOW, s_quotient);
  z = INUM(x)/z;
#ifdef BADIVSGNS
  {
# if (__TURBOC__==1)
    long t = ((y<0) ? -INUM(x) : INUM(x))%INUM(y);
# else
    long t = INUM(x)%INUM(y);
# endif
    if (t==0) ;
    else if (t < 0)
      if (x < 0) ;
      else z--;
    else if (x < 0) z++;
  }
#endif
  if (!FIXABLE(z))
#ifdef BIGDIG
    return long2big(z);
#else
  wta(x, (char *)OVFLOW, s_quotient);
#endif
  return MAKINUM(z);
}
