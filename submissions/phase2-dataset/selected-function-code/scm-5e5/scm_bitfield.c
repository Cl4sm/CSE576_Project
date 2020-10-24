     SCM n, start, end;
{
  int sign;
  ASRTER(INUMP(start), start, ARG2, s_bitfield);
  ASRTER(INUMP(end), end, ARG3, s_bitfield);
  start = INUM(start); end = INUM(end);
  ASRTER(end >= start, MAKINUM(end), OUTOFRANGE, s_bitfield);
#ifdef BIGDIG
  if (NINUMP(n)) {
    BIGDIG *ds;
    sizet i, nd;
    ASRTER(NIMP(n) && BIGP(n), n, ARG1, s_bitfield);
    sign = BIGSIGN(n);
  big:
    if (sign) n = scm_copy_big_2scomp(n, (sizet)end, 0);
    n = scm_big_ash(n, -start);
    if (INUMP(n)) {
      if (end - start >= LONG_BIT - 2) return n;
      return MAKINUM(INUM(n) & ((1L<<(end - start)) - 1));
    }
    nd = NUMDIGS(n);
    ds = BDIGITS(n);
    i = (end - start) / BITSPERDIG;
    if (i >= nd) return n;
    ds[i] &= ((1 << ((end - start) % BITSPERDIG)) - 1);
    for (++i; i < nd; i++) ds[i] = 0;
    return normbig(n);
  }
  if (end >= LONG_BIT - 2) {
    sign = INUM(n) < 0;
    goto big;
  }
#else
  ASRTER(INUMP(n), n, ARG1, s_bitfield);
  ASRTER(end < LONG_BIT - 2, MAKINUM(end), OUTOFRANGE, s_bitfield);
#endif
  return MAKINUM((INUM(n)>>start) & ((1L<<(end - start)) - 1));
}
