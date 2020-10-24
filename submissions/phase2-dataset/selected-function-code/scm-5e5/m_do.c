     SCM xorig, env, ctxt;
{
  SCM x = CDR(xorig), bdg, bdgs, test, body;
  SCM vars = IM_DO, inits = EOL, steps = EOL;
  int len = ilength(x);
  ASSYNT(len >= 2, xorig, s_test, s_do);
  bdgs = scm_check_linum(CAR(x), 0L);
  ASSYNT(ilength(bdgs) >= 0, CAR(x), s_bindings, s_do);
  while NIMP(bdgs) {
    bdg = scm_check_linum(CAR(bdgs), 0L);
    len = ilength(bdg);
    ASSYNT(2==len || 3==len, CAR(bdgs), s_bindings, s_do);
    vars = cons(CAR(bdg), vars); /* variable */
    bdg = CDR(bdg);
    inits = cons(CAR(bdg), inits);
    bdg = CDR(bdg);
    steps = cons(IMP(bdg) ? CAR(vars) : CAR(bdg), steps);
    bdgs = CDR(bdgs);
  }
  if (IMP(vars)) vars = EOL;
  inits = m_bindings(vars, inits, env, ctxt);
  env = EXTEND_ENV(vars, env);
  steps = m_bindings(vars, steps, env, ctxt);
  x = CDR(x);
  test = scm_check_linum(CAR(x), 0L);
  ASSYNT(ilength(test) >= 1, CAR(x), s_test, s_do);
  if (IMP(CDR(test))) test = cons(CAR(test), list_unspecified);
  ASSYNT(ilength(CDR(x))>=0, xorig, s_expression, s_do);
  varcheck(vars, IM_DO, s_variable);
  body = scm_check_linum(CDR(x), 0L);
  x = cons2(test, m_seq(body, env, i_side_effect), steps);
  x = cons2(env, inits, x);
  return cons(IM_DO, x);
}
