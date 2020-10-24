SCM m_case(xorig, env, ctxt)
     SCM xorig, env, ctxt;
{
  SCM clause, key_expr, x = CDR(xorig);
  SCM s, keys = EOL, action, actions = EOL, else_action = list_unspecified;
  int opt = !scm_nullenv_p(env);

  ASSYNT(ilength(x) >= 2, xorig, s_clauses, s_case);
  key_expr = CAR(x);
  while(NIMP(x = CDR(x))) {
    clause = CAR(x);
    s = scm_check_linum(clause, 0L);
    ASSYNT(ilength(s) >= 2, clause /* xorig */, s_clauses, s_case);
    clause = s;
    if (TOPDENOTE_EQ(i_else, CAR(clause), env)) {
      ASSYNT(NULLP(CDR(x)), xorig, s_bad_else_clause, s_case);
      else_action = m_seq(CDR(clause), env, ctxt);
    }
    else {
      s = scm_check_linum(CAR(clause), 0L);
#ifdef MACRO
      s = scm_cp_list(s, 0);
      ASSYNT(!UNBNDP(s), CAR(clause) /* xorig */, s_clauses, s_case);
      DEFER_INTS;
      unpaint(&s);
      ALLOW_INTS;
#else
      ASSYNT(ilength(s) >= 0, CAR(clause) /* xorig */, s_clauses, s_case);
#endif
      action = m_seq(CDR(clause), env, ctxt);
      for (; NIMP(s); s = CDR(s)) {
	ASSYNT(FALSEP(memv(CAR(s), keys)), xorig, "duplicate key value", s_case);
	if (NIMP(CAR(s)) && NUMP(CAR(s))) opt = 0;
	keys = cons(CAR(s), keys);
	actions = cons(action, actions);
      }
    }
  }
  key_expr = m_expr(key_expr, env, i_test);
  if (opt && NIMP(*loc_atcase_aux) && !in_atcase_aux) {
    SCM argv[3];
    argv[0] = keys;
    argv[1] = actions;
    argv[2] = else_action;
    in_atcase_aux = !0;
    x = scm_cvapply(*loc_atcase_aux, 3L, argv);
    in_atcase_aux = 0;		/* disabled after one error.  C'est la vie. */
    if (NIMP(x) && CONSP(x)) {
      s = CAR(x);
      if (INUMP(s) && INUM(s) >= 0 && INUM(s) <= 2)
	return cons2(MAKISYMVAL(IM_CASE, INUM(s)), key_expr, CDR(x));
    }
  }
  keys = cons(UNSPECIFIED, keys);
  actions = cons(else_action, actions);
  return cons2(IM_CASE, key_expr,
	       cons2(vector(keys), vector(actions), EOL));
}
