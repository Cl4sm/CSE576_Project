SCM scm_scope_trace(env)
     SCM env;
{
  SCM ef, file = UNDEFINED;
  int fprinted = 0;
  if (UNBNDP(env))
    env = scm_current_env();
  else if (NIMP(env) && CLOSUREP(env))
    env = CAR(CODE(env));
  if (scm_nullenv_p(env))
    lputs("\n; in top level environment.", cur_errp);
  else
    lputs("\n; in scope:", cur_errp);
#ifdef CAUTIOUS
  if (NIMP(ef=scm_env_getprop(SCM_ENV_FILENAME, env))) {
    file = CAR(ef);
  }
#endif
  for (; NIMP(env); env = CDR(env)) {
    if (NCONSP(env)) {
    badenv:
      lputs("\n; corrupted environment ", cur_errp);
      scm_iprin1(env, cur_errp, 1);
      return UNSPECIFIED;
    }
    ef = CAR(env);
    if (SCM_LINUMP(ef)) {
      lputs("\n;   ", cur_errp);
      scm_line_msg(file, ef, cur_errp);
      fprinted++;
    }
    else if (INUMP(ef)) {
      ASRTGO(NIMP(env) && CONSP(env), badenv);
      env = CDR(env);
#ifdef CAUTIOUS
      switch (ef) {
      default: break;
      case SCM_ENV_PROCNAME:
	lputs("  procedure ", cur_errp);
	scm_iprin1(CAR(env), cur_errp, 1);
	break;
      }
#endif
    }
    else if (NIMP(ef) && CONSP(ef) && NIMP(CAR(ef)) && CONSP(CAR(ef))) {
      lputs("\n;   ", cur_errp);
      scm_iprin1(CAR(ef), cur_errp, 1);
      lputs("  syntax bindings", cur_errp);
    }
    else {
      lputs("\n;   ", cur_errp);
      scm_iprin1(ef, cur_errp, 1);
    }
  }
#ifdef CAUTIOUS
  if (NIMP(file) && !fprinted) {
    lputs("\n; defined by ", cur_errp);
    if (NIMP(file) && STRINGP(file))
      lputs("load: ", cur_errp);
    scm_iprin1(file, cur_errp, 1);
    lputc('\n', cur_errp);
  }
#endif
  return UNSPECIFIED;
}
