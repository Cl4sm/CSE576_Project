SCM scm_logbitp(index, j1)
     SCM index, j1;
{
  ASRTER(INUMP(index) && INUM(index) >= 0, index, ARG1, s_logbitp);
#ifdef BIGDIG
  if (NINUMP(j1)) {
    ASRTER(NIMP(j1) && BIGP(j1), j1, ARG2, s_logbitp);
    if (NUMDIGS(j1) * BITSPERDIG < INUM(index)) return BOOL_F;
    else if (BIGSIGN(j1)) {
      long num = -1;
      sizet i = 0;
      BIGDIG *x = BDIGITS(j1);
      sizet nx = INUM(index)/BITSPERDIG;
      while (!0) {
	num += x[i];
	if (nx==i++)
	  return ((1L << (INUM(index)%BITSPERDIG)) & num) ? BOOL_F : BOOL_T;
	if (num < 0) num = -1;
	else num = 0;
      }
    }
    else return (BDIGITS(j1)[INUM(index)/BITSPERDIG] &
		 (1L << (INUM(index)%BITSPERDIG))) ? BOOL_T : BOOL_F;
  }
#else
  ASRTER(INUMP(j1), j1, ARG2, s_logbitp);
#endif
  if (INUM(index) >= LONG_BIT) return j1 < 0 ? BOOL_T : BOOL_F;
  return ((1L << INUM(index)) & INUM(j1)) ? BOOL_T : BOOL_F;
}
