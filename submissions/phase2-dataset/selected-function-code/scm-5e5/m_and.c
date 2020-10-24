     SCM xorig, env, ctxt;
{
  SCM x = CDR(xorig);
  int len = ilength(x);
  ASSYNT(len >= 0, xorig, s_test, s_and);
 tail:
  switch (len) {
  default:
    if (FALSEP(CAR(x))) return BOOL_F;
    if (constant_p(CAR(x))) {
      x = CDR(x);
      len--;
      goto tail;
    }
    return cons(IM_AND, x);
  case 1: return CAR(x);
  case 0: return BOOL_T;
  }
}
