     SCM x, y;
{
  if (UNBNDP(y)) {
    if (UNBNDP(x)) return MAKINUM(-1);
#ifndef RECKLESS
    if (!(NUMBERP(x)))
    badx: wta(x, (char *)ARG1, s_logand);
#endif
    return x;
  }
#ifdef BIGDIG
  if (NINUMP(x)) {
    SCM t;
    ASRTGO(NIMP(x) && BIGP(x), badx);
    if (INUMP(y)) {t = x; x = y; y = t; goto intbig;}
    ASRTGO(NIMP(y) && BIGP(y), bady);
    if (NUMDIGS(x) > NUMDIGS(y)) {t = x; x = y; y = t;}
    if ((BIGSIGN(x)) && BIGSIGN(y))
      return scm_big_ior(BDIGITS(x), NUMDIGS(x), 0x0100, y);
    return scm_big_and(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y, 0);
  }
  if (NINUMP(y)) {
# ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
    bady: wta(y, (char *)ARG2, s_logand);
# endif
  intbig: {
# ifndef DIGSTOOBIG
    long z = pseudolong(INUM(x));
    if ((x < 0) && BIGSIGN(y))
      return scm_big_ior((BIGDIG *)&z, DIGSPERLONG, 0x0100, y);
    return scm_big_and((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y,
			  0);
# else
    BIGDIG zdigs[DIGSPERLONG];
    longdigs(INUM(x), zdigs);
    if ((x < 0) && BIGSIGN(y))
      return scm_big_ior(zdigs, DIGSPERLONG, 0x0100, y);
    return scm_big_and(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y, 0);
# endif
  }}
#else
  ASRTGO(INUMP(x), badx);
  ASRTER(INUMP(y), y, ARG2, s_logand);
#endif
  return MAKINUM(INUM(x) & INUM(y));
}
