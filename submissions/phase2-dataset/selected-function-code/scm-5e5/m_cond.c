SCM m_cond(xorig, env, ctxt)
     SCM xorig, env, ctxt;
{
  SCM s, clause, cdrx = scm_cp_list(CDR(xorig), 1), x = cdrx;
  int len = ilength(x);
  ASSYNT(!UNBNDP(cdrx), xorig, s_clauses, s_cond);
  while(NIMP(x)) {
    clause = scm_check_linum(CAR(x), 0L);
    len = ilength(clause);
    ASSYNT(len >= 1, CAR(x), s_clauses, s_cond);
    if (TOPDENOTE_EQ(i_else, CAR(clause), env)) {
      ASSYNT(NULLP(CDR(x)) && len >= 2, xorig, s_bad_else_clause, s_cond);
      clause = cons(BOOL_T, m_seq(CDR(clause), env, ctxt));
    }
    else {
      s = CDR(clause);
      if (len >= 2 && TOPDENOTE_EQ(i_arrow, CAR(s), env)) {
	ASSYNT(3==len && NIMP(CAR(CDR(s))), clause, "bad recipient", s_cond);
	clause = cons2(CAR(clause), IM_ARROW, CDR(s));
      }
      else
	clause = cons(CAR(clause), m_seq(s, env, ctxt));
    }
    CAR(x) = clause;
    x = CDR(x);
  }
  return cons(IM_COND, cdrx);
}
