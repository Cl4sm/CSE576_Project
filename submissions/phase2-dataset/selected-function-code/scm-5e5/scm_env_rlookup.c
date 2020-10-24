     SCM addr, stenv;
     const char *what;
{
  SCM env, fr;
  int icdrp;
  unsigned int idist, iframe;
  if (IMP(addr)) {
    if (!ILOCP(addr)) return BOOL_F;
    iframe = IFRAME(addr);
    idist = IDIST(addr);
    icdrp = ICDRP(addr) && 1;
  }
  else {
    if (!ISYMP(CAR(addr))) return BOOL_F;
    icdrp = 0;
    switch (ISYMNUM(CAR(addr))) {
    default: return BOOL_F;
    case (ISYMNUM(IM_FARLOC_CDR)):
      icdrp = 1;
    case (ISYMNUM(IM_FARLOC_CAR)):
      fr = CDR(addr);
      iframe = INUM(CAR(fr));
      idist = INUM(CDR(fr));
      break;
    }
  }
  for (env = stenv; NIMP(env); env = CDR(env)) {
    fr = CAR(env);
    if (INUMP(fr)) {
      ASRTER(NIMP(env) && CONSP(env), stenv, s_badenv, what);
      env = CDR(env);
      continue;
    }
    if (SCM_LINUMP(fr)) continue;
    if (NIMP(fr) && CONSP(fr) && IMP(CAR(fr))) continue;
    if (0==iframe--) {
      while (idist--) {
	if (IMP(fr) || NCONSP(fr)) return BOOL_F;
	fr = CDR(fr);
      }
      if (!icdrp) {
	if (NIMP(fr) && CONSP(fr))
	  fr = CAR(fr);
	else
	  fr = BOOL_F;
      }
      if (NIMP(fr) && IDENTP(fr)) return fr;
      return BOOL_F;
    }
  }
  return BOOL_F;
}
