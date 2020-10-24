static int princodeptr(exp, port, writing)
     SCM exp;
     SCM port;
     int writing;
{
  SCM env = CAR(CDR(exp));
  lputs("#<code ", port);
  exp = CDR(CDR(exp));
  if (IMP(exp) || NCONSP(exp))
    scm_princode(exp, env, port, writing);
  else {
    while (!0) {
      if (IMP(exp) || NCONSP(exp)) break;
      scm_princode(CAR(exp), env, port, writing);
      lputc(' ', port);
      exp = CDR(exp);
    }
    if (NNULLP(exp)) {
      lputs(" . ", port);
      scm_princode(exp, env, port, writing);
    }
  }
  lputc('>', port);
  return !0;
}
