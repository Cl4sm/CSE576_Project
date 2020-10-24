     SCM xorig, env, ctxt;
{
  SCM x = CDR(xorig);
  int len = ilength(x);
  ASSYNT(len >= 0, xorig, s_test, s_or);
 tail:
  switch (len) {
  default:
    if (FALSEP(CAR(x))) {
      x = CDR(x);
      len--;
      goto tail;
    }
    if (constant_p(CAR(x)))
      return CAR(x);
    return cons(IM_OR, x);
  case 1: return CAR(x);
  case 0: return BOOL_F;
  }
}
