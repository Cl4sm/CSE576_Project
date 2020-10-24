     SCM n;
{
  register unsigned long c = 0;
  register long nn;
  unsigned int l = 4;
#ifdef BIGDIG
  if (NINUMP(n)) {
    BIGDIG *ds, d;
    ASRTER(NIMP(n) && BIGP(n), n, ARG1, s_intlength);
    if (BIGSIGN(n)) return scm_intlength(difference(MAKINUM(-1L), n));
    ds = BDIGITS(n);
    d = ds[c = NUMDIGS(n)-1];
    for (c *= BITSPERDIG; d; d >>= 4) {c += 4; l = ilentab[15 & d];}
    return MAKINUM(c - 4 + l);
  }
#else
  ASRTER(INUMP(n), n, ARG1, s_intlength);
#endif
  if ((nn = INUM(n)) < 0) nn = -1 - nn;
  for (;nn; nn >>= 4) {c += 4; l = ilentab[15 & nn];}
  return MAKINUM(c - 4 + l);
}
