     SCM var, stenv;
{
  SCM s, frame, env = stenv;
#ifdef MACRO
  SCM mark = IDENT_ENV(var);
  if (NIMP(mark)) mark = CAR(mark);
#endif
  for (; NIMP(env); env = CDR(env)) {
    frame = CAR(env);
#ifdef MACRO
    if (frame==mark) {
      var = IDENT_PARENT(var);
      mark = IDENT_ENV(var);
      if (NIMP(mark)) mark = CAR(mark);
    }
#endif
    if (IMP(frame)) {
      if (INUMP(frame)) {
#ifndef RECKLESS
	if (!(NIMP(env) && CONSP(env))) {
	badenv: wta(stenv, s_badenv, s_env_annotation);
	}
#endif
	env = CDR(env);
      }
      continue;
    }
#ifdef MACRO
    if (NIMP(frame) && CONSP(frame) && BOOL_F==CAR(frame)) {
      /* syntax binding */
      s = assq(var, CDR(frame));
      if (NIMP(s)) goto local_out;
      continue;
    }
#endif
    for (; NIMP(frame); frame = CDR(frame)) {
      if (NCONSP(frame)) {
	if (var==frame)
	  goto local_out;
	break;
      }
      if (CAR(frame)==var) {
      local_out:
	env = CDR(env);
	if (IMP(env)) return BOOL_T;
	if (SCM_ENV_ANNOTATION != CAR(env)) return BOOL_T;
	env = CDR(env);
	ASRTGO(NIMP(env), badenv);
	s = assq(var, CAR(env));
	if (NIMP(s)) return s;
	return BOOL_T;
      }
      ASRTGO(CONSP(frame), badenv);
    }
  }
  ASRTGO(NULLP(env), badenv);
  return BOOL_F;
}
