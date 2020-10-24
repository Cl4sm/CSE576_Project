void scm_princode(code, env, port, writing)
     SCM code, env, port;
     int writing;
{
  SCM oenv = env, name, init, bdgs, x = code;
  if (UNBNDP(env)) {
    scm_iprin1(code, port, writing);
    return;
  }
 tail:
  if (IMP(x)) {
    if (ILOCP(x)) {
    local:
      name = scm_env_rlookup(x, env, "princode");
      if (FALSEP(name)) goto gencase;
      lputs("#@", port);
      scm_iprin1(name, port, writing);
      return;
    }
    else
      goto gencase;
  }
  if (SCM_GLOCP(x)) {
    scm_iprin1(x, port, writing);
    return;
  }
  switch (TYP7(x)) {
  default: gencase:
    scm_iprin1(x, port, writing);
    return;
  gencode:
  case tcs_cons_gloc:
  case tcs_cons_nimcar:
  case tcs_cons_iloc:
  case (127 & IM_OR): case (127 & IM_AND): case (127 & IM_BEGIN):
  case (127 & IM_SET): case (127 & IM_COND): case (127 & IM_CASE):
  case (127 & IM_IF):
    lputc('(', port);
    scm_princode(CAR(x), env, port, writing);
  body:
    x = CDR(x);
  no_cdr:
    for (; NNULLP(x); x = CDR(x)) {
      if (IMP(x) || NECONSP(x)) {
	lputs(" . ", port);
	scm_iprin1(x, port, writing);
	break;
      }
      lputc(' ', port);
      scm_princode(CAR(x), env, port, writing);
    }
    lputc(')', port);
    return;
  case (127 & IM_LAMBDA):
    x = prinhead(x, port, writing);
    env = CAR(x);
    bdgs = SCM_ENV_FORMALS(env);
    if (IMP(bdgs) || NECONSP(bdgs))
      scm_iprin1(bdgs, port, writing);
    else {
      lputc('(', port);
      while (!0) {
	if (NECONSP(bdgs)) break;
	scm_iprin1(CAR(bdgs), port, writing);
	if (NIMP(bdgs = CDR(bdgs)))
	  lputc(' ', port);
	else break;
      }
      if (NIMP(bdgs)) {
	lputs(". ", port);
	scm_iprin1(bdgs, port, writing);
      }
      lputc(')', port);
    }
    goto body;
  case (127 & IM_LETREC):
  case (127 & IM_LET):
    x = prinhead(x, port, writing);
    env = CAR(x);
    prinbindings(SCM_ENV_FORMALS(env),
		 CAR(CDR(x)), (TYP7(x)==(127 & IM_LET) ? oenv: env),
		 UNDEFINED, UNDEFINED, port, writing);
    x = CDR(x);
    goto body;
  case (127 & IM_LETSTAR):
    x = prinhead(x, port, writing);
    lputc('(', port);
    if (NIMP(bdgs = CAR(x))) {
      oenv = CAR(bdgs);
      bdgs = CDR(bdgs);
      while (!0) {
	init = CAR(bdgs);
	bdgs = CDR(bdgs);
	env = CAR(bdgs);
	lputc('(', port);
	scm_iprin1(SCM_ENV_FORMALS(env), port, writing);
	lputc(' ', port);
	scm_princode(init, oenv, port, writing);
	oenv = env;
	lputc(')', port);
	if (IMP(bdgs = CDR(bdgs)))
	  break;
	lputc(' ', port);
      }
    }
    lputs(") ", port);
    goto body;
  case (127 & IM_DO):
    {
      /* (#@do (env (init ...) (test ...) (body ...) step ...)) */
      SCM test, steps;
      x = prinhead(x, port, writing);
      env = CAR(x);
      x = CDR(x);
      init = CAR(x);
      x = CDR(x);
      test = CAR(x);
      x = CDR(x);
      steps = CDR(x);
      x = CAR(x);
      prinbindings(SCM_ENV_FORMALS(env), init, oenv, steps, env,
		   port, writing);
      scm_princode(test, env, port, writing);
      lputc(' ', port);
      goto no_cdr;
    }
  case (127 & IM_FUNCALL):
    lputc('(', port);
    x = CDR(x);
    scm_princode(CAR(x), env, port, writing);
    goto body;
  case (127 & MAKISYM(0)):
    if (!ISYMP(CAR(x))) goto gencode;
    switch (ISYMNUM(CAR(x))) {
    default:
      goto gencode;
    case ISYMNUM(IM_LINUM):
      x = CDR(x);
      goto tail;
    case ISYMNUM(IM_FARLOC_CAR):
    case ISYMNUM(IM_FARLOC_CDR):
      goto local;
    }
  }
}
