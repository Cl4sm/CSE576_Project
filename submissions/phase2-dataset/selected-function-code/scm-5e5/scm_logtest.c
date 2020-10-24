SCM scm_logtest(x, y)
     SCM x, y;
{
#ifndef RECKLESS
    if (!(NUMBERP(x)))
    badx: wta(x, (char *)ARG1, s_logtest);
#endif
#ifdef BIGDIG
  if (NINUMP(x)) {
    SCM t;
    ASRTGO(NIMP(x) && BIGP(x), badx);
    if (INUMP(y)) {t = x; x = y; y = t; goto intbig;}
    ASRTGO(NIMP(y) && BIGP(y), bady);
    if (NUMDIGS(x) > NUMDIGS(y)) {t = x; x = y; y = t;}
    return scm_big_test(BDIGITS(x), NUMDIGS(x), BIGSIGN(x), y);
  }
  if (NINUMP(y)) {
# ifndef RECKLESS
    if (!(NIMP(y) && BIGP(y)))
    bady: wta(y, (char *)ARG2, s_logtest);
# endif
  intbig: {
# ifndef DIGSTOOBIG
    long z = pseudolong(INUM(x));
    return scm_big_test((BIGDIG *)&z, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y);
# else
    BIGDIG zdigs[DIGSPERLONG];
    longdigs(INUM(x), zdigs);
    return scm_big_test(zdigs, DIGSPERLONG, (x < 0) ? 0x0100 : 0, y);
# endif
  }}
#else
  ASRTGO(INUMP(x), badx);
  ASRTER(INUMP(y), y, ARG2, s_logtest);
#endif
  return (INUM(x) & INUM(y)) ? BOOL_T : BOOL_F;
}
