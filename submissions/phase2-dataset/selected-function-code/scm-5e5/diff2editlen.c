     SCM Fp, A, Args;		/* B, P_lim */
{
  SCM aprot, bprot;
  fp_procs *procs;
  int p_lim;
  int m, n;
  int *fp;
  void *a, *b;
  ASRTER(2==ilength(Args), Args, WNA, s_d2el);
  fp = array2addr(Fp, MAKINUM(-32), ARG1, s_d2el);
  aprot = array_prot(A);
  a = array2addr(A, aprot, ARG2, s_d2el);
  ASRTER(NFALSEP(aprot), aprot, ARG2, s_d2el);
  m = INUM(CAR(array_dims(A)));
  bprot = array_prot(CAR(Args));
  b = array2addr(CAR(Args), bprot, ARG3, s_d2el);
  ASRTER(NFALSEP(bprot), bprot, ARG3, s_d2el);
  n = INUM(CAR(array_dims(CAR(Args))));
  Args = CDR(Args);
  ASRTER(INUMP(CAR(Args)), CAR(Args), ARG4, s_d2el);
  p_lim = INUM(CAR(Args));
  ASRTER(aprot==bprot, bprot, s_incomp, s_d2el);
  procs = raprot2procs(aprot, s_d2el);
  {
    int maxdx = 0 > (p_lim)
      ?1+(n)
      :1+(p_lim)+((n)-(m));
    int mindx = 0 > (p_lim)
      ?-(1+(m))
      :-(1+(p_lim));
    int res;
    fp_init(fp, -(mindx), -1, mindx, maxdx);
    res = fp_compare(fp, -(mindx), 0, a, m, b, n,
		     procs->array_refsEql_P, p_lim);
    return (-1==res) ? BOOL_F : MAKINUM(res);
  }
}
