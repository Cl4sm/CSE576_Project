SCM scm_apply_cxr(proc, arg1)
     SCM proc, arg1;
{
  double y;
#ifdef FLOATS
  if (SUBRF(proc)) {
    if (INUMP(arg1)) {
      y = DSUBRF(proc)((double) INUM(arg1));
      goto ret;
    }
    ASRTGO(NIMP(arg1), floerr);
    if (REALP(arg1)) {
      y = DSUBRF(proc)(REALPART(arg1));
    ret:
      if (y==y) return makdbl(y, 0.0);
      goto floerr;
    }
# ifdef BIGDIG
    if (BIGP(arg1)) {
      y = DSUBRF(proc)(big2dbl(arg1));
      goto ret;
    }
# endif
  floerr:
    wta(arg1, (char *)ARG1, SNAME(proc));
  }
#endif
  {
    int op = CXR_OP(proc);
#ifndef RECKLESS
    SCM x = arg1;
#endif
    while (op) {
      ASRTER(NIMP(arg1) && CONSP(arg1),
	     x, ARG1, SNAME(proc));
      arg1 = (1 & op ? CAR(arg1) : CDR(arg1));
      op >>= 2;
    }
    return arg1;
  }
}
