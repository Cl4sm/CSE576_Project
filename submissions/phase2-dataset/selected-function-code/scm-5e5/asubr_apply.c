     SCM proc, arg1, arg2, arg3, args;
{
  switch TYP7(proc) {
  case tc7_asubr:
    arg1 = SUBRF(proc)(arg1, arg2);
    arg1 = SUBRF(proc)(arg1, arg3);
    while NIMP(args) {
      arg1 = SUBRF(proc)(arg1, CAR(args));
      args = CDR(args);
    }
    return arg1;
  case tc7_rpsubr:
    if (FALSEP(SUBRF(proc)(arg1, arg2))) return BOOL_F;
    while (!0) {
      if (FALSEP(SUBRF(proc)(arg2, arg3))) return BOOL_F;
      if (IMP(args)) return BOOL_T;
      arg2 = arg3;
      arg3 = CAR(args);
      args = CDR(args);
    }
  default: return UNDEFINED;
  }
}
