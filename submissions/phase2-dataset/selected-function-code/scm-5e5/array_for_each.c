SCM array_for_each(proc, ra0, lra)
     SCM proc, ra0, lra;
{
  long narg = ilength(lra) + 1;
 tail:
#ifndef RECKLESS
  scm_arity_check(proc, narg, s_array_for_each);
#endif
  switch TYP7(proc) {
  default: gencase:
    ramapc(rafe, proc, ra0, lra, s_array_for_each);
    return UNSPECIFIED;
#if 1 /* def CCLO */
  case tc7_specfun:
    if (tc16_cclo==TYP16(proc)) {
      lra = cons(ra0, lra);
      ra0 = sc2array(proc, ra0, EOL);
      proc = CCLO_SUBR(proc);
      narg++;
      goto tail;
    }
    goto gencase;
#endif
  }
}
