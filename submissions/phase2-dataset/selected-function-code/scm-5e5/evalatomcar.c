     SCM x;
     int toplevelp;
{
  SCM ret;
  switch TYP7(CAR(x)) {
  default:
    everr(x, STATIC_ENV, CAR(x), "Cannot evaluate: ", "", 0);
  lookup:
  case tcs_symbols:
    ret = scm_lookupval(x, !0);
    if (KEYWORDP(ret)) {
      SCM argv[3];
      SCM mac = KEYWORD_MACRO(ret);
      argv[0] = CAR(x);
      argv[1] = STATIC_ENV;
      argv[2] = EOL;
      switch (MAC_TYPE(mac) & ~MAC_PRIMITIVE) {
      default:
#ifdef MACRO
	if (!toplevelp)
	  everr(x, argv[1], argv[0], s_badkey, "", 0);
#endif
	return ret;
      case MAC_IDMACRO:
	ret = scm_cvapply(CDR(mac), 3L, argv);
	CAR(x) = ret;
	return evalcar(x);
      }
    }
    return ret;
  case tc7_vector:
#ifndef RECKLESS
    if (2 <= verbose) scm_warn("unquoted ", s_vector, CAR(x));
#endif
    ret = cons2(IM_QUOTE, CAR(x), EOL);
    CAR(x) = ret;
    return CAR(CDR(ret));
  case tc7_smob:
#ifdef MACRO
    if (M_IDENTP(CAR(x))) goto lookup;
#endif
	/* fall through */
  case tcs_uves:
    return CAR(x);
  }
}
