     SCM x, y;
{
  if (UNBNDP(y)) {
    if (UNBNDP(x)) return INUM0;
#ifndef RECKLESS
    if (!(NUMBERP(x)))
    badx: wta(x, (char *)ARG1, s_logxor);
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
    return scm_big_xor(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y);
  }
  if (NINUMP(y)) {
# ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
    bady: wta(y, (char *)ARG2, s_logxor);
# endif
  intbig: {
# ifndef DIGSTOOBIG
    long z = pseudolong(INUM(x));
    return scm_big_xor((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y);
# else
    BIGDIG zdigs[DIGSPERLONG];
    longdigs(INUM(x), zdigs);
    return scm_big_xor(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y);
# endif
  }}
#else
  ASRTGO(INUMP(x), badx);
  ASRTER(INUMP(y), y, ARG2, s_logxor);
#endif
  return (x ^ y) + INUM0;
}
