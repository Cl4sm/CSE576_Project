     SCM imm, xorig, x, *vars, *inits;
{
  SCM clause, bdgs, *varloc = vars, *initloc = inits;
  int len = ilength(x);
#ifdef MACRO
  const char *what = CHARS(ident2sym(CAR(xorig)));
#else
  const char *what = CHARS(CAR(xorig));
#endif
  *varloc = imm;
  *initloc = EOL;
  ASSYNT(len >= 2, UNDEFINED, s_body, what);
  bdgs = scm_check_linum(CAR(x), 0L);
  ASSYNT(ilength(bdgs) >= 0, bdgs, s_bindings, what);
  while NIMP(bdgs) {
    clause = scm_check_linum(CAR(bdgs), 0L);
    ASSYNT(2==ilength(clause), clause, s_bindings, what);
    ASSYNT(NIMP(CAR(clause)) && IDENTP(CAR(clause)), CAR(clause),
	   s_variable, what);
    *varloc = cons(CAR(clause), imm);
    varloc = &CDR(*varloc);
    *initloc = cons(CAR(CDR(clause)), EOL);
    initloc = &CDR(*initloc);
    bdgs = CDR(bdgs);
  }
  x = CDR(x);
  ASSYNT(ilength(x)>0, scm_wrapcode(x, EOL) /* xorig */, s_body, what);
  if (IMP(*vars)) *vars = EOL;
  return x;
}
