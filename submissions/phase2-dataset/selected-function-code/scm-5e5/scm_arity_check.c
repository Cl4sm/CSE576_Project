int scm_arity_check(proc, argc, what)
     SCM proc;
     long argc;
     const char *what;
{
  SCM p = proc;
  if (IMP(p) || argc < 0) goto badproc;
 cclo_tail:
  switch TYP7(p) {
  default:
  badproc:
    if (what) wta(proc, s_wtap, what);
    return 0;
  wrongnumargs:
    if (what) wta(proc, (char *)WNA, what);
    return 0;
  case tc7_subr_0: ASRTGO(0==argc, wrongnumargs) return !0;
  case tc7_contin:
    if (IM_VALUES_TOKEN == CONT(proc)->other.stkframe[1]) return !0;
				/* else fall through */
  case tc7_cxr:
  case tc7_subr_1: ASRTGO(1==argc, wrongnumargs) return !0;
  case tc7_subr_1o: ASRTGO(0==argc || 1==argc, wrongnumargs) return !0;
  case tc7_subr_2: ASRTGO(2==argc, wrongnumargs) return !0;
  case tc7_subr_2o: ASRTGO( 1==argc || 2==argc, wrongnumargs) return !0;
  case tc7_subr_3: ASRTGO(3==argc, wrongnumargs) return !0;
  case tc7_rpsubr:
  case tc7_asubr:
  case tc7_lsubr: return !0;
  case tc7_lsubr_2: ASRTGO(2<=argc, wrongnumargs) return !0;
  case tc7_specfun:
    switch TYP16(proc) {
    default: wta(proc, "internal error", "scm_arity_check");
    case tc16_apply: ASRTGO(2<=argc, wrongnumargs); return !0;
    case tc16_call_cc:
    case tc16_eval: ASRTGO(1==argc, wrongnumargs); /* fall through */
    case tc16_values: return !0;
    case tc16_call_wv: ASRTGO(2==argc, wrongnumargs); return !0;
# ifdef CCLO
    case tc16_cclo:
      p = CCLO_SUBR(p);
      argc++;
      goto cclo_tail;
# endif
    }
  case tcs_closures:
    {
      SCM formals = SCM_ENV_FORMALS(CAR(CODE(p)));
      while (argc--) {
	ASRTGO(NIMP(formals), wrongnumargs);
	if (CONSP(formals))
	  formals = CDR(formals);
	else
	  return !0;
      }
      ASRTGO(IMP(formals) || NCONSP(formals), wrongnumargs);
      return !0;
    }
  }
}
