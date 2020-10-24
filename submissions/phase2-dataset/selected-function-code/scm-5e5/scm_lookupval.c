static SCM scm_lookupval(vloc, memo)
     SCM vloc;
     int memo;
{
  SCM val, env = STATIC_ENV, var = CAR(vloc);
  SCM addr = scm_env_lookup(var, env);
  if (IMP(addr)) { /* local ref */
    DEFER_INTS_EGC;
    val = *ilookup(addr);
  }
#ifdef MACRO
  else if (NIMP(addr) && IM_KEYWORD==CAR(addr)) /* local macro binding */
    val = addr;
#endif
  else if (ISYMP(CAR(addr))) {	/* local ref (farloc) */
    DEFER_INTS_EGC;
    val = *farlookup(addr);
  }
  else {			/* global ref */
#ifdef MACRO
    ASRTER(SYMBOLP(addr), var, s_escaped, "");
#endif
    addr = sym2vcell(addr);
    val = CDR(addr);
    addr += tc3_cons_gloc;
  }
  ASRTER(!UNBNDP(val) && val != undefineds, var, s_unbnd, "");
  if (memo && !KEYWORDP(val))	/* Don't memoize forms to be macroexpanded. */
    CAR(vloc) = addr;
  return val;
}
