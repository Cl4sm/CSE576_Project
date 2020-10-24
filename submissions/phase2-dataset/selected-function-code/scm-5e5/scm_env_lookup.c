     SCM var, stenv;
{
  SCM  frame, env = stenv;
  long icdr = 0L;
  unsigned int idist, iframe = 0;
#ifdef MACRO
  SCM mark = IDENT_ENV(var);
  if (NIMP(mark)) mark = CAR(mark);
#endif
  for (; NIMP(env); env = CDR(env)) {
    idist = 0;
    frame = CAR(env);
#ifdef MACRO
    if (frame==mark) {
      var = IDENT_PARENT(var);
      mark = IDENT_ENV(var);
      if (NIMP(mark)) mark = CAR(mark);
    }
#endif
    if (IMP(frame)) {
      if (NULLP(frame)) iframe++;
      else if (INUMP(frame)) {
#ifndef RECKLESS
	if (!(NIMP(env) && CONSP(env))) {
	badenv: wta(stenv, s_badenv, s_lookup);
	}
#endif
	env = CDR(env);
      }
      else {
	ASRTGO(SCM_LINUMP(frame), badenv);
      }
      continue;
    }
#ifdef MACRO
    if (NIMP(frame) && CONSP(frame) && SCM_ENV_SYNTAX==CAR(frame)) {
      /* syntax binding */
      SCM s = assq(var, CDR(frame));
      if (NIMP(s)) return cons(IM_KEYWORD, CDR(s));
      continue;
    }
#endif
    for (; NIMP(frame); frame = CDR(frame)) {
      if (NCONSP(frame)) {
	if (var==frame) {
	  icdr = ICDR;
	  goto local_out;
	}
	break;
      }
      if (CAR(frame)==var) {
      local_out:
#ifndef TEST_FARLOC
	var = MAKILOC(iframe, idist) + icdr;
	if (iframe==IFRAME(var) && idist==IDIST(var))
	  return var;
	else
#endif
	  return cons2(icdr ? IM_FARLOC_CDR : IM_FARLOC_CAR,
		       MAKINUM(iframe), MAKINUM(idist));
      }
      ASRTGO(CONSP(frame), badenv);
      idist++;
    }
    iframe++;
  }
  ASRTGO(NULLP(env), badenv);
#ifdef MACRO
  while (M_IDENTP(var)) {
    if (IMP(IDENT_ENV(var)))
      var = IDENT_PARENT(var);
    else break;
  }
#endif
  return var;
}
