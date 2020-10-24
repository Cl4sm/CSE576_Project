static SCM macroexp1(xorig, env, ctxt, mode)
     SCM xorig, env, ctxt;
     int mode;
{
  SCM x = xorig, linum, proc = UNDEFINED, res = UNDEFINED;
#ifndef RECKLESS
  SCM trace = scm_trace, trace_env = scm_trace_env;
  long argc;
  const char *what = s_wtap;
  MACROEXP_TRACE(xorig, env);
#endif
  x = scm_check_linum(xorig, &linum);
  if (IMP(x) || NCONSP(x)) {	/* Happens for unquoted vectors. */
    if (NIMP(x))
      x = evalatomcar(cons(x, EOL), 0);
    x = cons2(IM_QUOTE, x, EOL);
    goto retx;
  }
  else if (IDENTP(x)) {		/* Happens for @macroexpand1 */
    proc = x;
    x = cons(proc, EOL);
  }
  else
    proc = CAR(x);
  ASRTGO(NIMP(proc), errout);
  if (CONSP(proc)) {
    if (mode < 3) {
      x = xorig;
      goto retx;
    }
    if (NIMP(CAR(proc)))
      proc = macroexp1(cons(CAR(proc), CDR(proc)), env, i_procedure, mode);
    if ((127L & IM_LAMBDA)==(127L & CAR(proc))) {
      SCM nenv = CAR(CDR(proc));
      SCM formals = SCM_ENV_FORMALS(nenv);
#ifndef RECKLESS
      if (badargsp(formals, CDR(x))) {
	what = (char *)WNA;
	proc = CAR(x);
	goto errout;
      }
#endif
      res = CDR(x);
      if (ilength(formals) >= 0) {
	x = cons2(IM_LET, nenv, cons(res, CDR(CDR(proc))));
	goto retx;
      }
    }
#ifndef RECKLESS
    if (ilength(x) < 0) {
      what = s_expr;
      goto errout;
    }
#endif
    x = cons2(IM_FUNCALL, proc, CDR(x));
    goto retx;
  }
  ASRTGO(IDENTP(proc), errout);
 macro_tail:
  res = proc;			/* For nicer error message. */
  if (mode >= 3) {
    x = cons(CAR(x), CDR(x));
    proc = scm_lookupval(x, !0);
  }
  else {
    proc = scm_env_lookup(proc, env);
    if (IMP(proc)) {		/* local binding */
      x = scm_add_linum(linum, x);
      goto retx;
    }
    if (CONSP(proc))		/* local syntax binding. */
      proc = CDR(proc);
    else if (SYMBOLP(proc))	/* global variable */
      proc = CDR(sym2vcell(proc));
  }
  if (KEYWORDP(proc)) {
    SCM argv[3];
    long argc = 2;
    proc = KEYWORD_MACRO(proc);
    argv[0] = x;
    argv[1] = env;
    argv[2] = ctxt;
    switch (MAC_TYPE(proc)) {
    case MAC_MACRO: case MAC_MACRO | MAC_PRIMITIVE:
    case MAC_ACRO: case MAC_ACRO | MAC_PRIMITIVE:
	/* This means non-memoizing macros can't expand into internal defines.
	   That's ok with me. */
      if (mode > 1)
	x = cons2(IM_ACRO_CALL, CAR(x), CDR(x));
      goto retx;
    case MAC_MMACRO | MAC_PRIMITIVE:
    case MAC_IDMACRO | MAC_PRIMITIVE:
      if (0==mode ||
          (1==mode && f_define != CDR(proc) && f_begin != CDR(proc))) {
        x = scm_add_linum(linum, x);
        goto retx;
      }
      argv[2] = ctxt;
      argc = 3;
				/* fall through */
    case MAC_MMACRO:
    case MAC_IDMACRO:
      argv[0] = x;
      argv[1] = env;
      x = scm_cvapply(CDR(proc), argc, argv);
      if (ilength(x) <= 0)
	x = cons2((0==mode ? TOPRENAME(i_begin): IM_BEGIN), x, EOL);
      break;
#ifdef MAC_INLINE		/* FIXME this is broken */
    case MAC_INLINE:
      {
	int depth = env_depth();
	res = CDR(proc);
	depth -= INUM(CAR(res));
	res = CDR(res);
	x = cons2(MAKISYMVAL(IM_LET, depth),
		     CAR(res), cons(CDR(x), CDR(res)));
	break;
      }
#endif
    }
    MACROEXP_TRACE(xorig, env);
    x = scm_check_linum(x, 0L);
    if (NIMP(CAR(x)) && IDENTP(CAR(x))) {
      proc = CAR(x);
      goto macro_tail;
    }
#ifndef RECKLESS
    if (IM_DEFINE==CAR(x) && (mode != 1) && !scm_nullenv_p(env)) {
      what = s_placement;
      proc = res = i_define;
    errout:
      if (!UNBNDP(res))
	CAR(x) = res; /* FIXME may not be right for @macroexpand1 */
      if (UNBNDP(proc) && NIMP(x) && CONSP(x))
	proc = CAR(x);
      scm_experr(proc, what, "");
    }
#endif
  }
  else {		/* not a macro expression, car is identifier */
    if (0 == mode)
      x = BOOL_F;
    else if (mode <=2 )
      x = scm_add_linum(linum, x);
#ifndef RECKLESS
    else if (mode >= 3) {
      argc = ilength(CDR(x));
      if (! scm_arity_check(proc, argc, (char *)0)) {
	if (argc < 0) {
	  what = s_expr;
	  proc = x;
	}
	else
	  what = FALSEP(procedurep(proc)) ? s_wtap : (char *)WNA;
	goto errout;
      }
      for (proc = CDR(x); NIMP(proc); proc = CDR(proc)) {
	res = CAR(proc);
	if (NIMP(res)) {
	  if (IDENTP(res))
	    scm_lookupval(proc, !0);
	  else if (CONSP(res))
	    macroexp1(res, env, i_argument, mode);
	}
      }
    }
#endif
  }
 retx:
  if (mode >= 3 && x != xorig) {
    DEFER_INTS;
    CAR(xorig) = CAR(x);
    CDR(xorig) = CDR(x);
    x = xorig;
    ALLOW_INTS;
  }
  MACROEXP_TRACE(trace, trace_env); /* restore */
  return x;
}
