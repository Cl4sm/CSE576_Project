SCM scm_copybit(index, j1, bit)
     SCM index, j1, bit;
{
  ASRTER(INUMP(index) && INUM(index) >= 0, index, ARG1, s_copybit);
#ifdef BIGDIG
  {
    SCM res;
    BIGDIG *rds;
    sizet i = INUM(index);
    int sign;
    if (!INUMP(j1)) {
      ASRTER(NIMP(j1) && BIGP(j1), j1, ARG2, s_copybit);
      sign = BIGSIGN(j1);
    ovflow:
      res = scm_copy_big_2scomp(j1, i + 1, sign);
      rds = BDIGITS(res);
      if (NFALSEP(bit))
	rds[i / BITSPERDIG] |= 1 << (i % BITSPERDIG);
      else
	rds[i / BITSPERDIG] &= ~(1 << (i % BITSPERDIG));
      if (sign) scm_2scomp1(res);
      return normbig(res);
    }
    if (i >= LONG_BIT - 3) {
      sign = INUM(j1) < 0 ? 0x0100 : 0;
      goto ovflow;
    }
  }
#else
  ASRTER(INUMP(j1), j1, ARG2, s_copybit);
  ASRTER(INUM(index) < LONG_BIT - 3, index, OUTOFRANGE, s_copybit);
#endif
  if (NFALSEP(bit))
    return MAKINUM(INUM(j1) | (1L << INUM(index)));
  else
    return MAKINUM(INUM(j1) & (~(1L << INUM(index))));
}
