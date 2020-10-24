     SCM xorig, env, ctxt;
{
  SCM proc, body, vars, inits, x = CDR(xorig);
  ASSYNT(ilength(x) >= 2, xorig, s_body, s_let);
  proc = CAR(x);
  if (NIMP(proc) && IDENTP(proc)) { /* named let, build equiv letrec */
    x = CDR(x);
    body = m_parse_let(IM_LET, xorig, x, &vars, &inits);
    x = cons2(TOPRENAME(i_lambda), vars, body);
    x = cons2(i_let, cons(cons2(proc, x, EOL), EOL), cons(proc, EOL));
    return cons(m_letrec1(IM_LET, x, env, ctxt), inits);
  }
				/* vanilla let */
  body = m_parse_let(IM_LET, xorig, x, &vars, &inits);
  varcheck(vars, IM_LET, s_variable);
  if (IMP(vars))
    return m_let_null(body, env, ctxt);
  if (IMP(CDR(vars)))		/* single binding, let* is faster */
    return m_letstar1(IM_LET, vars, inits, body, env, ctxt);
  inits = m_bindings(vars, inits, env, ctxt);
  env = EXTEND_ENV(vars, env);
  return cons2(IM_LET, env, cons(inits, m_body(body, env, ctxt)));
}
