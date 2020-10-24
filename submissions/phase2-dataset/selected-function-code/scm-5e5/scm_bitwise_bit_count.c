SCM scm_bitwise_bit_count(n)
     SCM n;
{
  register unsigned long c = 0;
  register long nn;
#ifdef BIGDIG
  if (NINUMP(n)) {
    sizet i; BIGDIG *ds, d;
    ASRTER(NIMP(n) && BIGP(n), n, ARG1, s_bitwise_bit_count);
    if (BIGSIGN(n))
      return scm_lognot(scm_bitwise_bit_count(difference(MAKINUM(-1L), n)));
    ds = BDIGITS(n);
    for (i = NUMDIGS(n); i--; )
      for (d = ds[i]; d; d >>= 4) c += logtab[15 & d];
    if (BIGSIGN(n))
      return MAKINUM(-1 - c);
    return MAKINUM(c);
  }
#else
  ASRTER(INUMP(n), n, ARG1, s_bitwise_bit_count);
#endif
  if ((nn = INUM(n)) < 0) nn = -1 - nn;
  for (; nn; nn >>= 4) c += logtab[15 & nn];
  if (n < 0)
    return MAKINUM(-1 - c);
  return MAKINUM(c);
}
