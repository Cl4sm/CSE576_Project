     SCM x;
{
#ifdef BIGDIG
  if (NINUMP(x)) {
    ASRTER(NIMP(x) && BIGP(x), x, ARG1, s_abs);
    if (TYP16(x)==tc16_bigpos) return x;
    return copybig(x, 0);
  }
#else
  ASRTER(INUMP(x), x, ARG1, s_abs);
#endif
  if (INUM(x) >= 0) return x;
  x = -INUM(x);
  if (!POSFIXABLE(x))
#ifdef BIGDIG
    return long2big(x);
#else
    wta(MAKINUM(-x), (char *)OVFLOW, s_abs);
#endif
  return MAKINUM(x);
}
