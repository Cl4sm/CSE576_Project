     SCM vars, op;
     const char *what;
{
  SCM v1, vs;
  char *opstr = ISYMCHARS(op) + 2;
  int argc = 0;
  vars = scm_check_linum(vars, 0L);
  for (; NIMP(vars) && CONSP(vars); vars = CDR(vars)) {
    argc++;
#ifndef RECKLESS
    v1 = CAR(vars);
    if (IMP(v1) || !IDENTP(v1))
      badvar: scm_experr(v1, what, opstr);
    for (vs = CDR(vars); NIMP(vs) && CONSP(vs); vs = CDR(vs)) {
      if (v1==CAR(vs)) {
	nonuniq:
	what = "non-unique bindings";
	goto badvar;
      }
    }
    if (v1==vs) goto nonuniq;
#endif
  }
		/* argc of 3 means no rest argument, 3+ required arguments */
  if (NULLP(vars) || ISYMP(vars)) return argc > 3 ? 3 : argc;
  ASRTGO(NIMP(vars) && IDENTP(vars), badvar);
  return argc > 2 ? 2 : argc;
}
