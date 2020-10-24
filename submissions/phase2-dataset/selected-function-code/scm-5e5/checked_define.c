static void checked_define(name, val, what)
     SCM name, val;
     const char *what;
{
  SCM old, vcell;
#ifdef MACRO
  while (M_IDENTP(name)) {
    ASRTER(IMP(IDENT_ENV(name)), name, s_escaped, what);
    name = IDENT_PARENT(name);
  }
#endif
  vcell = sym2vcell(name);
  old = CDR(vcell);
#ifndef RECKLESS
    if ('@'==CHARS(name)[0] && UNDEFINED != old)
      scm_warn(s_redefining, "internal name ", name);
    if (KEYWORDP(old)) {
      if (1 <= verbose && built_inp(name, KEYWORD_MACRO(old)))
	scm_warn(s_redefining, s_built_in_syntax, name);
      else if (3 <= verbose)
	scm_warn(s_redefining, s_syntax, name);
    }
    else if (2 <= verbose && built_inp(name, old) && (old != val))
      scm_warn(s_redefining, "built-in ", name);
    else if (5 <= verbose && UNDEFINED != old)
      scm_warn(s_redefining, "", name);
#endif
    CDR(vcell) = val;
}
