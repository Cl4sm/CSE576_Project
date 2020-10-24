     SCM n, cnt;
{
  SCM res = INUM(n);
  ASRTER(INUMP(cnt), cnt, ARG2, s_ash);
  cnt = INUM(cnt);
  if (INUMP(n)) {
    if (cnt < 0) {
      if (-cnt >= LONG_BIT) return INUM0;
      return MAKINUM(SRS(res, -cnt));
    }
    if (cnt >= LONG_BIT) goto ovflow;
    res = MAKINUM(res<<cnt);
    if (INUM(res)>>cnt != INUM(n))
      goto ovflow;
    else
      return res;
  }
#ifdef BIGDIG
  ASRTER(NIMP(n) && BIGP(n), n, ARG1, s_ash);
 ovflow:
  if (0==cnt) return n;
  return scm_big_ash(n, cnt);
#else
 ovflow:
  wta(n, INUMP(n) ? (char *)OVFLOW : (char *)ARG1, s_ash);
  return UNSPECIFIED;	/* kill warning */
#endif
}
