     SCM pv, modes;
{
  SCM z;
  long flags;
  static long arities[] = {1, 1, 0, 0, 0};
#ifndef RECKLESS
  int i;
  if (! (NIMP(pv) && VECTORP(pv) && 5==LENGTH(pv)))
    badarg: wta(pv, (char *)ARG1, s_mksfpt);
  for (i = 0; i < 5; i++) {
    ASRTGO(FALSEP(VELTS(pv)[i]) ||
	   scm_arity_check(VELTS(pv)[i], arities[i], (char *)0),
	   badarg);
  }
#endif
  ASRTER(NIMP(modes) && (STRINGP(modes) || SYMBOLP(modes)), modes, ARG2, s_mksfpt);
  flags = mode_bits(CHARS(modes), (char *)0);
  ASRTER(flags, modes, ARG2, s_mksfpt);
  DEFER_INTS;
  z = scm_port_entry((FILE *)pv, tc16_sfport, flags);
  ALLOW_INTS;
  return z;
}
