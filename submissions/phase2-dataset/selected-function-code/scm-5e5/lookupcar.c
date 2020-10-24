static SCM *lookupcar(vloc)
     SCM vloc;
{
  SCM *pv, val, var = CAR(vloc), env = STATIC_ENV;
  SCM addr = scm_env_lookup(var, env);
  if (IMP(addr) || ISYMP(CAR(addr))) { /* local ref */
    DEFER_INTS_EGC;
    pv = IMP(addr) ? ilookup(addr) : farlookup(addr);
  }
#ifdef MACRO
# ifndef RECKLESS
  else if (NIMP(addr) && IM_KEYWORD==CAR(addr)) { /* local macro binding */
  badkey: wta(var, s_badkey, "");
  }
# endif
#endif
  else {			/* global ref */
#ifdef MACRO
    ASRTER(SYMBOLP(addr), var, s_escaped, "");
#endif
    val = sym2vcell(addr);
    addr = val + tc3_cons_gloc;
    pv = &CDR(val);
#ifdef MACRO
    ASRTGO(!KEYWORDP(*pv), badkey);
#endif
  }
  ASRTER(!UNBNDP(*pv) && undefineds != *pv, var, s_unbnd, "");
  CAR(vloc) = addr;
  return pv;
}
