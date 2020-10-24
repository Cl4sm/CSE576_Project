SCM for_each(proc, arg1, args)
     SCM proc, arg1, args;
{
  SCM heap_ve, auto_ve[5], auto_ave[5];
  SCM *ve = auto_ve, *ave = auto_ave;
  long i, n = ilength(args) + 1;
  scm_protect_temp(&heap_ve);  /* Keep heap_ve from being optimized away. */
  if (NULLP(arg1)) return UNSPECIFIED;
#ifndef RECKLESS
  scm_arity_check(proc, n, s_for_each);
#endif
  ASRTER(NIMP(arg1), arg1, ARG2, s_for_each);
#ifdef CCLO
  if (tc16_cclo==TYP16(proc)) {
    args = cons(arg1, args);
    arg1 = cons(proc, EOL);
    SETCDR(arg1, arg1);		/* circular list */
    proc = CCLO_SUBR(proc);
    n++;
  }
#endif
  if (n > 5) {
    heap_ve = make_vector(MAKINUM(2*n), BOOL_F);
    ve = VELTS(heap_ve);
    ave = &(ve[n]);
  }
  ve[0] = arg1;
  ASRTER(NIMP(ve[0]), arg1, ARG2, s_for_each);
  for (i = 1; i < n; i++) {
    ve[i] = CAR(args);
    ASRTER(NIMP(ve[i]), args, ARGn, s_for_each);
    args = CDR(args);
  }
  while (1) {
    arg1 = EOL;
    for (i = n-1;i >= 0;i--) {
      if (IMP(ve[i])) {
	return UNSPECIFIED;
      }
      ASRTER(CONSP(ve[i]), ve[i], 0==i ? ARG2 : ARGn, s_for_each);
      ave[i] = CAR(ve[i]);
      ve[i] = CDR(ve[i]);
    }
    scm_cvapply(proc, n, ave);
  }
}
