static SCM m_body(xorig, env, ctxt)
     SCM xorig, env, ctxt;
{
  SCM form, denv = env, x = xorig, defs = EOL;
  const char *what = 0; /* Should this be passed in? */
  ASRTSYNTAX(ilength(xorig) >= 1, s_expression);
  while NIMP(x) {
    form = scm_check_linum(CAR(x), 0L);
    if (IMP(form) || NCONSP(form)) break;
    if (IMP(CAR(form))) break;
    if (! IDENTP(CAR(form))) break;
    form = macroexp1(CAR(x), denv, i_check_defines, 1);
    if (IM_DEFINE==CAR(form)) {
      defs = cons(CDR(form), defs);
      x = CDR(x);
    }
    else if (IM_BEGIN==CAR(form)) {
      form = CDR(form);
      x = CDR(x);
      if (IMP(x))
	x = form;
      else if (UNSPECIFIED==CAR(form) && IMP(CDR(form)))
	;
      else
	x = append(cons2(form, x, EOL));
    }
    else if (NIMP(defs)) {
      break;
    }
    else {
      /* Doesn't work when m_body recursively called
	 x = cons(form, m_seq(CDR(x), env, ctxt)); */
      x = cons(form, CDR(x));
      break;
    }
  }
  ASSYNT(ilength(x) > 0, xorig, s_body, what);
  if (IMP(defs)) return x;
  return cons(m_letrec1(IM_DEFINE, cons2(i_define, defs, x), env, ctxt), EOL);
}
