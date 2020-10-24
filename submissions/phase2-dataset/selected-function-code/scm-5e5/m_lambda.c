     SCM xorig, env, ctxt;
{
  SCM x = CDR(xorig), formals;
#ifdef CAUTIOUS
  SCM name, linum;
#endif
  int argc;
  ASRTER(ilength(x) > 1, x, s_body, s_lambda);
  formals = CAR(x);
  argc = varcheck(formals, IM_LAMBDA, s_formals);
  formals = scm_check_linum(formals, 0L);
  if (argc > 3) argc = 3;
  x = CDR(x);
  if (NIMP(CDR(x)) && NIMP(CAR(x)) && STRINGP(CAR(x))) {
    env = scm_env_addprop(SCM_ENV_DOC, CAR(x), env);
    x = CDR(x);
  }
#ifdef CAUTIOUS
  if (NIMP(ctxt) && i_bind==CAR(ctxt)) {
    ctxt = CDR(ctxt);
    name = CAR(ctxt);
  }
  else
    name = i_anon;
  if (NIMP(scm_trace) && xorig==scm_check_linum(scm_trace, &linum))
    if (!UNBNDP(linum)) env = EXTEND_ENV(linum, env);
  env = scm_env_addprop(SCM_ENV_PROCNAME, name, env);
#endif
  env = EXTEND_ENV(formals, env);
  return cons2(MAKISYMVAL(IM_LAMBDA, argc), env, m_body(x, env, EOL));
}
