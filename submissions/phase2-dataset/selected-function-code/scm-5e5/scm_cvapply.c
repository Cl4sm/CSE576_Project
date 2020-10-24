SCM scm_cvapply(proc, n, argv)
     SCM proc, *argv;
     long n;
{
  SCM res;
  long i;
 tail:
  ALLOW_INTS_EGC;
  switch TYP7(proc) {
  default: return UNSPECIFIED;
  case tc7_subr_2o:
    if (1==n) return SUBRF(proc)(argv[0], UNDEFINED);
    /* Fall through */
  case tc7_subr_2:
    return SUBRF(proc)(argv[0], argv[1]);
  case tc7_subr_0:
    return SUBRF(proc)();
  case tc7_subr_1o:
    if (0==n) return SUBRF(proc)(UNDEFINED);
    /* Fall through */
  case tc7_subr_1:
    return SUBRF(proc)(argv[0]);
  case tc7_cxr:
    return scm_apply_cxr(proc, argv[0]);
  case tc7_subr_3:
    return SUBRF(proc)(argv[0], argv[1], argv[2]);
  case tc7_lsubr:
    return SUBRF(proc)(0==n ? EOL : scm_v2lst(n, argv, EOL));
  case tc7_lsubr_2:
    return SUBRF(proc)(argv[0], argv[1],
		       2==n ? EOL : scm_v2lst(n-2, &argv[2], EOL));
  case tc7_asubr:
    if (1 >= n) return SUBRF(proc)(0==n ? UNDEFINED: argv[0], UNDEFINED);
    res = argv[0];
    for (i = 1; i < n; i++)
      res = SUBRF(proc)(res, argv[i]);
    return res;
  case tc7_rpsubr:
    if (1 >= n) return BOOL_T;
    for (i = 0; i < n-1; i++)
      if (FALSEP(SUBRF(proc)(argv[i], argv[i+1]))) return BOOL_F;
    return BOOL_T;
  case tcs_closures: {
    SCM p = proc;
    ENV_PUSH;
    i = ARGC(proc);
    if (3==i) {
      scm_env_tmp = EOL;
      ENV_V2LST(n, argv);
    }
    else {
      scm_env_tmp = (i < n) ? scm_v2lst(n-i, &argv[i], EOL) : EOL;
      if (i>0)
	ENV_V2LST(i, argv);
    }
    ENV_V2LST(1L, &p);
    res = ceval_1(f_evapply);
    return res;
  }
  case tc7_contin:
    if (1 == n) scm_dynthrow(proc, argv[0], UNDEFINED, EOL);
    goto call_apply;
  case tc7_specfun:
    if (tc16_apply==TYP16(proc)) {
      proc = argv[0];
      argv++;
      n--;
#ifndef RECKLESS
      scm_arity_check(proc, n, s_apply);
#endif
      goto tail;
    }
  call_apply:
    res = cons(proc, 0==n ? EOL : scm_v2lst(n, argv, EOL));
#ifdef CCLO
    proc = (TYP16(proc)==tc16_cclo ? CCLO_SUBR(proc) : f_apply_closure);
#else
    proc = f_apply_closure;
#endif
    return apply(proc, res, EOL);
  }
}
