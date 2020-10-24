     SCM x;
{
#ifdef GCC_SPARC_BUG
  SCM arg1;
#else
  struct {SCM arg_1;} t;
# define arg1 t.arg_1
#endif
  SCM arg2, arg3, proc;
  int envpp = 0;	/* 1 means an environment has been pushed in this
		   invocation of ceval_1, -1 means pushed and then popped. */
#ifdef CAUTIOUS
  SCM xorig;
#endif
  CHECK_STACK;
 loop: POLL;
#ifdef CAUTIOUS
  xorig = x;
#endif
#ifdef SCM_PROFILE
  eval_cases[TYP7(x)]++;
#endif
  switch TYP7(x) {
  case tcs_symbols:
    /* only happens when called at top level */
    x = evalatomcar(cons(x, UNDEFINED), !0);
    goto retx;
  case (127 & IM_AND):
    x = CDR(x);
    arg1 = x;
    while(NNULLP(arg1 = CDR(arg1)))
      if (FALSEP(EVALCAR(x))) {x = BOOL_F; goto retx;}
      else x = arg1;
    goto carloop;
 cdrxbegin:
  case (127 & IM_BEGIN):
    x = CDR(x);
 begin:
    arg1 = x;
    while(NNULLP(arg1 = CDR(arg1))) {
      if (NIMP(CAR(x))) ceval_1(CAR(x));
      x = arg1;
    }
 carloop:			/* eval car of last form in list */
    if (NCELLP(CAR(x))) {
      x = CAR(x);
      x = IMP(x) ? EVALIMP(x) : I_VAL(x);
    }
    else if (ATOMP(CAR(x)))
      x = evalatomcar(x, 0);
    else {
      x = CAR(x);
      goto loop;			/* tail recurse */
    }
 retx:
    ENV_MAY_POP(envpp, 0);
    ALLOW_INTS_EGC;
    return x;

  case (127 & IM_CASE):
    x = scm_case_selector(x);
    goto begin;
  case (127 & IM_COND):
    while(NIMP(x = CDR(x))) {
      proc = CAR(x);
      arg1 = EVALCAR(proc);
      if (NFALSEP(arg1)) {
	x = CDR(proc);
	if (NULLP(x)) {
	  x = arg1;
	  goto retx;
	}
	if (IM_ARROW != CAR(x)) goto begin;
	proc = CDR(x);
	proc = EVALCAR(proc);
	ASRTGO(NIMP(proc), badfun);
	goto evap1;
      }
    }
    x = UNSPECIFIED;
    goto retx;
  case (127 & IM_DO):
    ENV_MAY_PUSH(envpp);
    TRACE(x);
    x = CDR(x);
    ecache_evalx(CAR(CDR(x)));	/* inits */
    STATIC_ENV = CAR(x);
    EXTEND_VALENV;
    x = CDR(CDR(x));
    while (proc = CAR(x), FALSEP(EVALCAR(proc))) {
      for (proc = CAR(CDR(x));NIMP(proc);proc = CDR(proc)) {
	arg1 = CAR(proc);	/* body */
	SIDEVAL_1(arg1);
      }
      ecache_evalx(CDR(CDR(x))); /* steps */
      scm_env = CDR(scm_env);
      EXTEND_VALENV;
    }
    x = CDR(proc);
    if (NULLP(x)) {x = UNSPECIFIED; goto retx;}
    goto begin;
  case (127 & IM_IF):
    x = CDR(x);
    if (NFALSEP(EVALCAR(x))) x = CDR(x);
    else if (IMP(x = CDR(CDR(x)))) {x = UNSPECIFIED; goto retx;}
    goto carloop;
  case (127 & IM_LET):
    ENV_MAY_PUSH(envpp);
    TRACE(x);
#ifdef MAC_INLINE
    arg1 = CAR(x);
#endif
    x = CDR(x);
    ecache_evalx(CAR(CDR(x)));
#ifdef MAC_INLINE
    if (arg1 != IM_LET)	/* inline call */
      env_tail(ISYMVAL(arg1));
#endif
    STATIC_ENV = CAR(x);
    EXTEND_VALENV;
    x = CDR(x);
    goto cdrxbegin;
  case (127 & IM_LETREC):
    ENV_MAY_PUSH(envpp);
    TRACE(x);
    x = CDR(x);
    STATIC_ENV = CAR(x);
#if 0    /*
           The block below signals an error if any variable
           bound in a LETREC is referenced in any init.
         */
    scm_env_tmp = undefineds;
    EXTEND_VALENV;
    x = CDR(x);
    ecache_evalx(CAR(x));
    EGC_ROOT(scm_env);
    CAR(scm_env) = scm_env_tmp;

#else  /* The block below implements LETREC* */
    ecache_undefs(CAR(CAR(x)));
    EXTEND_VALENV;
    x = CDR(x);
    proc = CAR(x);
    while (NIMP(proc)) {
      arg1 = EVALCAR(proc);
      proc = CDR(proc);
      DEFER_INTS_EGC;
      CAR(scm_env_tmp) = arg1;
      scm_env_tmp = CDR(scm_env_tmp);
    }
#endif
    scm_env_tmp = EOL;
    goto cdrxbegin;
  case (127 & IM_LETSTAR):
    ENV_MAY_PUSH(envpp);
    TRACE(x);
    x = CDR(x);
    proc = CDR(CAR(x));
    /* No longer happens.
      if (IMP(proc)) {
        scm_env_tmp = EOL;
	EXTEND_VALENV;
	goto cdrxbegin;
	}
    */
    scm_env_tmp = EOL;		/* needed so multiple values cause an error
                               to be signaled when this is a top-level form. */
    do {
      scm_env_tmp = EVALCAR(proc);
      proc = CDR(proc);
      STATIC_ENV = CAR(proc);
      EXTEND_VALENV;
    } while NIMP(proc = CDR(proc));
    goto cdrxbegin;
  case (127 & IM_OR):
    x = CDR(x);
    arg1 = x;
    while(NNULLP(arg1 = CDR(arg1))) {
      x = EVALCAR(x);
      if (NFALSEP(x)) goto retx;
      x = arg1;
    }
    goto carloop;
  case (127 & IM_LAMBDA):
    x = closure(CDR(x), ISYMVAL(CAR(x)));
    goto retx;
  case (127 & IM_QUOTE):
    x = CAR(CDR(x));
    goto retx;
  case (127 & IM_SET):
    x = CDR(x);
    arg2 = EVALCAR(CDR(x));
    proc = CAR(x);
    switch (7 & (int)proc) {
    case 0:
      if (ECONSP(proc))
	if (ISYMP(CAR(proc))) *farlookup(proc) = arg2;
	else {
	  x = scm_multi_set(proc, arg2);
	  goto retx;
	}
      else *lookupcar(x) = arg2;
      break;
    case 1:
      I_VAL(proc) = arg2;
      break;
    case 4:
      *ilookup(proc) = arg2;
      break;
    }
#ifdef SICP
    x = arg2;
#else
    x = UNSPECIFIED;
#endif
    goto retx;
  case (127 & IM_FUNCALL):
    x = CDR(x);
    proc = ceval_1(CAR(x));
    break;
  case (127 & MAKISYM(0)):
    proc = CAR(x);
    ASRTGO(ISYMP(proc), badfun);
#ifdef SCM_PROFILE
    eval_cases_other[ISYMNUM(proc)]++;
#endif
    switch ISYMNUM(proc) {
    case (ISYMNUM(IM_APPLY)):
      x = CDR(x);
      proc = evalcar(x);
      ASRTGO(NIMP(proc), badfun);
      arg1 = evalcar(CDR(x));
      if (CLOSUREP(proc)) {
	ENV_MAY_PUSH(envpp);
	TRACE(x);
	scm_env_tmp = arg1;
#ifndef RECKLESS
	goto clo_checked;
#else
	goto clo_unchecked;
#endif
      }
      x = apply(proc, arg1, EOL);
      goto retx;
    case (ISYMNUM(IM_DELAY)):
      x = makprom(closure(CDR(x), 0));
      goto retx;
    case (ISYMNUM(IM_QUASIQUOTE)):
      ALLOW_INTS_EGC;
      x = iqq(CAR(CDR(x)));
      goto retx;
    case (ISYMNUM(IM_FARLOC_CAR)):
    case (ISYMNUM(IM_FARLOC_CDR)):
      x = *farlookup(x);
      goto retx;
    case (ISYMNUM(IM_EVAL_FOR_APPLY)):
      /* only happens when called from C-level apply or cvapply */
      envpp = 1;
      proc = CAR(scm_env_tmp);
      scm_env_tmp = CDR(scm_env_tmp);
      goto clo_unchecked;
    case (ISYMNUM(IM_LET_SYNTAX)):
      x = CDR(x);
      STATIC_ENV = CAR(x);
      goto cdrxbegin;
    case (ISYMNUM(IM_ACRO_CALL)):
      x = acro_call(x, STATIC_ENV);
      goto loop;
    case (ISYMNUM(IM_LINUM)):
#ifndef MEMOIZE_LOCALS
      x = CDR(x);               /* For non-memoizing case,
                                   just throw away line number. */
      goto loop;
#else
      goto expand;
#endif
    case (ISYMNUM(IM_DEFINE)):
      x = toplevel_define(x, STATIC_ENV);
      goto retx;
    case (ISYMNUM(IM_EVAL_VALUES)):
                                /* Push magic VALUES token on estk until
                                   tail call occurs.  Only happens when called
                                   from scm_eval_values. */
      ENV_MAY_PUSH(envpp);
      scm_env_tmp = EOL;
      goto cdrxbegin;
    /* new syntactic forms go here. */
    default:
      goto badfun;
    }
  default:
    proc = x;
  badfun:
#ifdef CAUTIOUS
    scm_trace = BOOL_F;
    everr(xorig, STATIC_ENV, proc, s_wtap, "", 0);
#else
    everr(x, STATIC_ENV, proc, s_wtap, "", 0);
#endif
  case tc7_vector:
  case tcs_uves:
  case tc7_smob:
    goto retx;
  case (127 & ILOC00):
    proc = *ilookup(CAR(x));
    break;
  case tcs_cons_gloc:
    proc = I_VAL(CAR(x));
    break;
  case tcs_cons_nimcar:
  expand:
    TOP_TRACE(x, STATIC_ENV);
#ifdef MEMOIZE_LOCALS
    x = macroexp1(x, STATIC_ENV, EOL, 3);
    goto loop;
#else
    if (ATOMP(CAR(x))) {
      proc = scm_lookupval(x, 0);
      if (KEYWORDP(proc)) {
	x = macroexp1(x, STATIC_ENV, EOL, 3);
	goto loop;
      }
    }
    else proc = ceval_1(CAR(x));
#endif
  }
    /* At this point proc is the evaluated procedure from the function
       position and x has the form which is being evaluated. */
  ASRTGO(NIMP(proc), badfun);
  scm_estk_ptr[0] = scm_env; /* For error reporting at wrongnumargs. */
  if (NULLP(CDR(x))) {
  evap0:
    TOP_TRACE(xorig, STATIC_ENV);
    ENV_MAY_POP(envpp, CLOSUREP(proc));
    ALLOW_INTS_EGC;
    switch TYP7(proc) { /* no arguments given */
    case tc7_subr_0:
      return SUBRF(proc)();
    case tc7_subr_1o:
      return SUBRF(proc) (UNDEFINED);
    case tc7_lsubr:
      return SUBRF(proc)(EOL);
    case tc7_rpsubr:
      return BOOL_T;
    case tc7_asubr:
      return SUBRF(proc)(UNDEFINED, UNDEFINED);
    case tcs_closures:
      DEFER_INTS_EGC;
      ENV_MAY_PUSH(envpp);
      scm_env_tmp = EOL;
#ifdef SCM_PROFILE
      eval_clo_cases[0][0]++;
#endif
#ifdef CAUTIOUS
      if (0!=ARGC(proc)) {
      clo_checked:
	arg1 = SCM_ENV_FORMALS(CAR(CODE(proc)));
	DEFER_INTS_EGC;
	arg2 = scm_env_tmp;
	while NIMP(arg1) {
	  if (NCONSP(arg1)) goto clo_unchecked;
	  if (IMP(arg2)) goto umwrongnumargs;
	  arg1 = CDR(arg1);
	  arg2 = CDR(arg2);
	}
	if (NNULLP(arg2)) goto umwrongnumargs;
      }
#else /* def CAUTIOUS */
    clo_checked:
#endif
    clo_unchecked:
      x = CODE(proc);
      scm_env = ENV(proc);
      STATIC_ENV = CAR(x);
      EXTEND_VALENV;
      TRACE(CDR(x));
      goto cdrxbegin;
    case tc7_specfun:
      switch TYP16(proc) {
	/* default: break; */
#ifdef CCLO
      case tc16_cclo:
	arg1 = proc;
	proc = CCLO_SUBR(proc);
	goto evap1;
#endif
      case tc16_values:
	return scm_values(UNDEFINED, UNDEFINED, EOL, s_values);
      }
    case tc7_contin:
      scm_dynthrow(proc, UNDEFINED, UNDEFINED, EOL);
    case tc7_subr_1:
    case tc7_subr_2:
    case tc7_subr_2o:
    case tc7_cxr:
    case tc7_subr_3:
    case tc7_lsubr_2:
    umwrongnumargs:
    wrongnumargs:
      if (envpp < 0) {
	scm_estk_ptr += SCM_ESTK_FRLEN;
	scm_env = scm_estk_ptr[0];
      }
#ifdef CAUTIOUS
      if (xorig==scm_trace) STATIC_ENV = scm_trace_env;
      TOP_TRACE(BOOL_F, BOOL_F);
      everr(xorig, STATIC_ENV, proc, (char *)WNA, "", 0);
#else
      everr(x, STATIC_ENV, proc, (char *)WNA, "", 0);
#endif
    default:
      goto badfun;
    }
  }
  x = CDR(x);
#ifdef CAUTIOUS
  if (IMP(x))
    goto wrongnumargs;
#endif
  arg1 = EVALCAR(x);
  x = CDR(x);
  if (NULLP(x)) {
    TOP_TRACE(xorig, STATIC_ENV);
evap1:
    ENV_MAY_POP(envpp, CLOSUREP(proc));
    ALLOW_INTS_EGC;
    switch TYP7(proc) { /* have one argument in arg1 */
    case tc7_subr_2o:
      return SUBRF(proc)(arg1, UNDEFINED);
    case tc7_subr_1:
    case tc7_subr_1o:
      return SUBRF(proc)(arg1);
    case tc7_cxr:
      return scm_apply_cxr(proc, arg1);
    case tc7_rpsubr:
      return BOOL_T;
    case tc7_asubr:
      return SUBRF(proc)(arg1, UNDEFINED);
    case tc7_lsubr:
      return SUBRF(proc)(cons(arg1, EOL));
    case tcs_closures:
      ENV_MAY_PUSH(envpp);
#ifdef SCM_PROFILE
      eval_clo_cases[1][ARGC(proc)]++;
#endif
      if (1==ARGC(proc)) {
	scm_env_cons(arg1, EOL);
	goto clo_unchecked;
      }
      else {
	scm_env_tmp = cons(arg1, EOL);
	goto clo_checked;
      }
    case tc7_contin:
      scm_dynthrow(proc, arg1, UNDEFINED, EOL);
    case tc7_specfun:
      switch TYP16(proc) {
      case tc16_call_cc:
	proc = arg1;
	DEFER_INTS_EGC;
	arg1 = scm_make_cont();
	EGC_ROOT(arg1);
	x = setjump(CONT(arg1)->jmpbuf);
	if (x) {
#ifdef SHORT_INT
	  x = (SCM)thrown_value;
#endif
#ifdef CHEAP_CONTINUATIONS
	  envpp = 0;
#endif
	  goto retx;
	}
	ASRTGO(NIMP(proc), badfun);
	goto evap1;
      case tc16_eval:
	ENV_MAY_PUSH(envpp);
	TRACE(arg1);
	STATIC_ENV = eval_env;
	scm_env = EOL;
	x = arg1;
	if (IMP(x)) goto retx;
	goto loop;
#ifdef CCLO
      case tc16_cclo:
	arg2 = UNDEFINED;
	goto cclon;
	/* arg2 = arg1;
	   arg1 = proc;
	   proc = CCLO_SUBR(proc);
	   goto evap2; */
#endif
      case tc16_values: return arg1;
      }
    case tc7_subr_2:
    case tc7_subr_0:
    case tc7_subr_3:
    case tc7_lsubr_2:
      goto wrongnumargs;
    default:
      goto badfun;
    }
  }
#ifdef CAUTIOUS
  if (IMP(x)) goto wrongnumargs;
#endif
  {				/* have two or more arguments */
    arg2 = EVALCAR(x);
    x = CDR(x);
    if (NULLP(x)) {		/* have two arguments */
      TOP_TRACE(xorig, STATIC_ENV);
  evap2:
      ENV_MAY_POP(envpp, CLOSUREP(proc));
      ALLOW_INTS_EGC;
      switch TYP7(proc) {
      case tc7_subr_2:
      case tc7_subr_2o:
	return SUBRF(proc)(arg1, arg2);
      case tc7_lsubr:
	return SUBRF(proc)(cons2(arg1, arg2, EOL));
      case tc7_lsubr_2:
	return SUBRF(proc)(arg1, arg2, EOL);
      case tc7_rpsubr:
      case tc7_asubr:
	return SUBRF(proc)(arg1, arg2);
      case tc7_specfun:
	switch TYP16(proc) {
	case tc16_apply:
	  proc = arg1;
	  ASRTGO(NIMP(proc), badfun);
	  if (NULLP(arg2)) goto evap0;
	  if (IMP(arg2) || NCONSP(arg2)) {
	    x = arg2;
	  badlst: wta(x, (char *)ARGn, s_apply);
	  }
	  arg1 = CAR(arg2);
	  x = CDR(arg2);
	apply3:
	  if (NULLP(x)) goto evap1;
	  ASRTGO(NIMP(x) && CONSP(x), badlst);
	  arg2 = CAR(x);
	  x = CDR(x);
	apply4:
	  if (NULLP(x)) goto evap2;
	  ASRTGO(NIMP(x) && CONSP(x), badlst);
	  arg3 = x;
	  x = scm_cp_list(CDR(x), 0);
#ifndef RECKLESS
	  if (UNBNDP(x)) {x = arg3; goto badlst;}
#endif
	  arg3 = CAR(arg3);
	  goto evap3;
#ifdef CCLO
	case tc16_cclo: cclon:
	  arg3 = arg2;
	  arg2 = arg1;
	  arg1 = proc;
	  proc = CCLO_SUBR(proc);
	  if (UNBNDP(arg3)) goto evap2;
	  goto evap3;
	  /* return apply(CCLO_SUBR(proc),
		       cons2(proc, arg1, cons(arg2, x)), EOL); */
#endif
	case tc16_values:
	  return scm_values(arg1, arg2, EOL, s_values);
	case tc16_call_wv:
	  ENV_MAY_PUSH(envpp);
	  scm_env_tmp = IM_VALUES_TOKEN; /* Magic value recognized by VALUES */
	  arg1 = apply(arg1, EOL, EOL);
	  proc = arg2;
	  DEFER_INTS_EGC;
	  if (IM_VALUES_TOKEN==scm_env_tmp) {
	    scm_env_tmp = EOL;
	    if (UNBNDP(arg1)) goto evap0;
	    goto evap1;
	  }
	  arg2 = CAR(scm_env_tmp);
	  x = CDR(scm_env_tmp);
	  goto apply4;   /* Jumping to apply code results in extra list copy
			    for >=3 args, but we want to minimize bloat. */
	}
      case tc7_subr_0:
      case tc7_cxr:
      case tc7_subr_1o:
      case tc7_subr_1:
      case tc7_subr_3:
      case tc7_contin:
	scm_dynthrow(proc, arg1, arg2, EOL);
	goto wrongnumargs;
      default:
	goto badfun;
      case tcs_closures:
	ENV_MAY_PUSH(envpp);
#ifdef SCM_PROFILE
	eval_clo_cases[2][ARGC(proc)]++;
#endif
	switch ARGC(proc) {
	case 2:
	  scm_env_cons2(arg1, arg2, EOL);
	  goto clo_unchecked;
	case 1:
	  scm_env_cons(arg1, cons(arg2, EOL));
	  goto clo_checked;
	case 0:
	case 3:		/* Error, will be caught at clo_checked: */
	  scm_env_tmp = cons2(arg1, arg2, EOL);
	  goto clo_checked;
	}
      }
    }
    {				/* have 3 or more arguments */
      arg3 = EVALCAR(x);
      x = CDR(x);
      if (NIMP(x)) {
	if (CLOSUREP(proc) && 3==ARGC(proc)) {
	  ALLOW_INTS_EGC;
	  ENV_MAY_PUSH(envpp);
	  if (ecache_eval_args(proc, arg1, arg2, arg3, x))
	    goto clo_unchecked;
	  goto umwrongnumargs;
	}
	x = eval_args(x);
      }
      TOP_TRACE(xorig, STATIC_ENV);
    evap3:
      ENV_MAY_POP(envpp, CLOSUREP(proc));
      ALLOW_INTS_EGC;
      switch TYP7(proc) {
      case tc7_subr_3:
	ASRTGO(NULLP(x), wrongnumargs);
	return SUBRF(proc)(arg1, arg2, arg3);
      case tc7_asubr:
      case tc7_rpsubr:
	return asubr_apply(proc, arg1, arg2, arg3, x);
	/* return apply(proc, cons2(arg1, arg2, cons(arg3, x)), EOL); */
      case tc7_lsubr_2:
	return SUBRF(proc)(arg1, arg2, cons(arg3, x));
      case tc7_lsubr:
	return SUBRF(proc)(cons2(arg1, arg2, cons(arg3, x)));
      case tcs_closures:
	ENV_MAY_PUSH(envpp);
#ifdef SCM_PROFILE
	eval_clo_cases[IMP(x)?3:4][ARGC(proc)]++;
#endif
	switch ARGC(proc) {
	case 3:
	  scm_env_cons3(arg1, arg2, arg3, x);
	  goto clo_checked;
	case 2:
	  scm_env_cons2(arg1, arg2, cons(arg3, x));
	  goto clo_checked;
	case 1:
	  scm_env_cons(arg1, cons2(arg2, arg3, x));
	  goto clo_checked;
	case 0:
	  scm_env_tmp = cons2(arg1, arg2, cons(arg3, x));
	  goto clo_checked;
	}
      case tc7_specfun:
	switch TYP16(proc) {
	case tc16_apply:
	  proc = arg1;
	  ASRTGO(NIMP(proc), badfun);
	  arg1 = arg2;
	  if (IMP(x)) {
	    x = arg3;
	    goto apply3;
	  }
	  arg2 = arg3;
	  if (IMP(CDR(x))) {
	    x = CAR(x);
	    goto apply4;
	  }
	  arg3 = CAR(x);
	  x = nconc2copy(CDR(x));
	  goto evap3;
#ifdef CCLO
	case tc16_cclo:
	  x = cons(arg3, x);
	  goto cclon;
#endif
	case tc16_values:
	  return scm_values(arg1, arg2, cons(arg3, x), s_values);
	}
      case tc7_contin:
	  scm_dynthrow(proc, arg1, arg2, cons(arg3, x));
      case tc7_subr_2:
      case tc7_subr_1o:
      case tc7_subr_2o:
      case tc7_subr_0:
      case tc7_cxr:
      case tc7_subr_1:
	goto wrongnumargs;
      default:
	goto badfun;
      }
    }
  }
#undef arg1
}
