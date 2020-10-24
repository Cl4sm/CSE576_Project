SCM force(x)
     SCM x;
{
 tail:
  ASRTGO(NIMP(x) && (TYP16(x)==tc16_promise), badx);
  switch (CAR(x)>>16) {
  default:
  badx: wta(x, (char *)ARG1, s_force);
  case 0:
    {
      SCM ans;
      int mv = (IM_VALUES_TOKEN==scm_env_tmp);
      ans = scm_cvapply(CDR(x), 0L, (SCM *)0);
      if (mv) {
	DEFER_INTS_EGC;
	if (IM_VALUES_TOKEN==scm_env_tmp) {
	  if (!UNBNDP(ans)) mv = 0;
	}
	else {
	  ans = cons2(ans, CAR(scm_env_tmp), CDR(scm_env_tmp));
	  scm_env_tmp = IM_VALUES_TOKEN;
	}
	ALLOW_INTS_EGC;
      }
      if (!((1L<<16) & CAR(x))) {
	DEFER_INTS;
	CDR(x) = ans;
	CAR(x) |= mv ? (3L<<16) : (1L<<16);
	ALLOW_INTS;
      }
      goto tail;
    }
  case 1: return CDR(x);
  case 3:
    x = CDR(x);
    if (UNBNDP(x)) return scm_values(UNDEFINED, UNDEFINED, EOL, s_force);
    return scm_values(CAR(x), CAR(CDR(x)), CDR(CDR(x)), s_force);
  }
}
