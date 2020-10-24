     SCM Edits, Fp, Args;	/* Ccrr, A, B; */
{
  SCM aprot, bprot;
  int *edits;
  int est;
  int *fp;
  int *ccrr;
  void *a, *b;
  int m, n;
  fp_procs *procs;
  ASRTER(3==ilength(Args), Args, WNA, s_d2es);
  edits = array2addr(Edits, MAKINUM(-32), ARG1, s_d2es);
  fp = array2addr(Fp, MAKINUM(-32), ARG2, s_d2es);
  ccrr = array2addr(CAR(Args), MAKINUM(-32), ARG3, s_d2es);
  Args = CDR(Args);
  aprot = array_prot(CAR(Args));
  a = array2addr(CAR(Args), aprot, ARG4, s_d2es);
  ASRTER(NFALSEP(aprot), aprot, ARG4, s_d2es);
  m = INUM(CAR(array_dims(CAR(Args))));
  Args = CDR(Args);
  bprot = array_prot(CAR(Args));
  b = array2addr(CAR(Args), bprot, ARG5, s_d2es);
  ASRTER(NFALSEP(bprot), bprot, ARG5, s_d2es);
  n = INUM(CAR(array_dims(CAR(Args))));
  ASRTER(aprot==bprot, bprot, s_incomp, s_d2es);
  procs = raprot2procs(aprot, s_d2es);
  est = INUM(CAR(array_dims(Edits)));
  {
    int p_lim = ((est)-((n)-(m)))/2;
    check_cost(s_d2es, est,
	       diff2et(fp, 1+(p_lim),
		       ccrr, a, 0, m, b, 0, n, edits, 0, 1, procs, p_lim));
    return UNSPECIFIED;
  }
}
