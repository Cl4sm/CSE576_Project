     const char *name;
     int req, opt, rst;
     SCM (*fcn)();
{
  switch GSUBR_MAKTYPE(req, opt, rst) {
  case GSUBR_MAKTYPE(0, 0, 0): return make_subr(name, tc7_subr_0, fcn);
  case GSUBR_MAKTYPE(1, 0, 0): return make_subr(name, tc7_subr_1, fcn);
  case GSUBR_MAKTYPE(0, 1, 0): return make_subr(name, tc7_subr_1o, fcn);
  case GSUBR_MAKTYPE(1, 1, 0): return make_subr(name, tc7_subr_2o, fcn);
  case GSUBR_MAKTYPE(2, 0, 0): return make_subr(name, tc7_subr_2, fcn);
  case GSUBR_MAKTYPE(3, 0, 0): return make_subr(name, tc7_subr_3, fcn);
  case GSUBR_MAKTYPE(0, 0, 1): return make_subr(name, tc7_lsubr, fcn);
  case GSUBR_MAKTYPE(2, 0, 1): return make_subr(name, tc7_lsubr_2, fcn);
  default:
    {
      SCM symcell = sysintern(name, UNDEFINED);
      SCM z = scm_maksubr(name, tc7_subr_0, fcn);
      SCM cclo = makcclo(f_gsubr_apply, 3L);
      ASRTER(GSUBR_MAX >= req + opt + rst, MAKINUM(req + opt + rst),
	     OUTOFRANGE, "make_gsubr");
      GSUBR_PROC(cclo) = z;
      GSUBR_TYPE(cclo) = MAKINUM(GSUBR_MAKTYPE(req, opt, rst));
      CDR(symcell) = cclo;
      return cclo;
    }
  }
}
