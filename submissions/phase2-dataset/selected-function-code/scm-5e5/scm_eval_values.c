SCM scm_eval_values(x, env, valenv)
     SCM x, env, valenv;
{
  SCM res;
  ENV_PUSH;
#ifdef CAUTIOUS
  scm_trace = BOOL_F;
#endif
  TRACE(x);
  STATIC_ENV = env;
  scm_env = valenv;
  scm_env_tmp = IM_VALUES_TOKEN;
  if (NIMP(x)) x = ceval_1(cons2(IM_EVAL_VALUES, x, EOL));
  DEFER_INTS_EGC;
  if (IM_VALUES_TOKEN==scm_env_tmp) {
    if (UNBNDP(x))
      res = EOL;
    else
      res = cons(x, EOL);
  }
  else
    res = cons2(x, CAR(scm_env_tmp), CDR(scm_env_tmp));
  ENV_POP;
  ALLOW_INTS_EGC;
  return res;
}
