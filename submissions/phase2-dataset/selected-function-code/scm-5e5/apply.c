SCM apply(proc, arg1, args)
     SCM proc, arg1, args;
{
  ASRTGO(NIMP(proc), badproc);
  if (NULLP(args))
    if (NULLP(arg1)) arg1 = UNDEFINED;
    else {
      args = CDR(arg1);
      arg1 = CAR(arg1);
    }
  else
    args = nconc2copy(args);
 cc_tail:
  ALLOW_INTS_EGC;
  switch TYP7(proc) {
  default:
  badproc:
    wta(proc, (char *)ARG1, s_apply);
  wrongnumargs:
    wta(proc, (char *)WNA, s_apply);
  case tc7_subr_2o:
    if (NULLP(args)) {
      args = UNDEFINED;
      return SUBRF(proc)(arg1, args);
    }
    /* Fall through */
  case tc7_subr_2:
    ASRTGO(NIMP(args) && NULLP(CDR(args)), wrongnumargs);
    args = CAR(args);
    return SUBRF(proc)(arg1, args);
  case tc7_subr_0:
    ASRTGO(UNBNDP(arg1), wrongnumargs);
    return SUBRF(proc)();
  case tc7_subr_1:
  case tc7_subr_1o:
    ASRTGO(NULLP(args), wrongnumargs);
    return SUBRF(proc)(arg1);
  case tc7_cxr:
    ASRTGO(NULLP(args), wrongnumargs);
    return scm_apply_cxr(proc, arg1);
  case tc7_subr_3:
    ASRTGO(NIMP(args) && NIMP(CDR(args)) && NULLP(CDR(CDR(args))),
	   wrongnumargs);
    return SUBRF(proc)(arg1, CAR(args), CAR(CDR(args)));
  case tc7_lsubr:
    return SUBRF(proc)(UNBNDP(arg1) ? EOL : cons(arg1, args));
  case tc7_lsubr_2:
    ASRTGO(NIMP(args) && CONSP(args), wrongnumargs);
    return SUBRF(proc)(arg1, CAR(args), CDR(args));
  case tc7_asubr:
    if (NULLP(args)) return SUBRF(proc)(arg1, UNDEFINED);
    while NIMP(args) {
      ASRTER(CONSP(args), args, ARG2, s_apply);
      arg1 = SUBRF(proc)(arg1, CAR(args));
      args = CDR(args);
    }
    return arg1;
  case tc7_rpsubr:
    if (NULLP(args)) return BOOL_T;
    while NIMP(args) {
      ASRTER(CONSP(args), args, ARG2, s_apply);
      if (FALSEP(SUBRF(proc)(arg1, CAR(args)))) return BOOL_F;
      arg1 = CAR(args);
      args = CDR(args);
    }
    return BOOL_T;
  case tcs_closures: {
    arg1 = (UNBNDP(arg1) ? EOL : cons(arg1, args));
#ifndef RECKLESS
    if (badargsp(SCM_ENV_FORMALS(CAR(CODE(proc))), arg1)) goto wrongnumargs;
#endif
    ENV_PUSH;
    scm_env_cons(proc, arg1);
    arg1 = ceval_1(f_evapply);
    return arg1;
  }
  case tc7_contin:
    if (NULLP(args)) scm_dynthrow(proc, arg1, UNDEFINED, EOL);
				/* else fall through */
  case tc7_specfun:
    args = UNBNDP(arg1) ? EOL : cons(arg1, args);
    arg1 = proc;
#ifdef CCLO
    proc = (TYP16(proc)==tc16_cclo ? CCLO_SUBR(proc) : f_apply_closure);
#else
    proc = f_apply_closure;
#endif
    goto cc_tail;
  }
}
