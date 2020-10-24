     SCM ra, proc;
{
  SCM hp_av, hp_indv, auto_av[5];
  SCM *av = &auto_av[0];
  long auto_indv[5];
  long *indv = &auto_indv[0];
  sizet i;
  ASRTER(NIMP(ra), ra, ARG1, s_array_imap);
  i = INUM(array_rank(ra));
#ifndef RECKLESS
    scm_arity_check(proc, i+0L, s_array_imap);
#endif
  if (i >= 5) {
    scm_protect_temp(&hp_av);
    scm_protect_temp(&hp_indv);
    hp_av = make_vector(MAKINUM(i), BOOL_F);
    av = VELTS(hp_av);
    hp_indv = make_uve(i+0L, MAKINUM(-32L));
    indv = (long *)VELTS(hp_indv);
  }
  switch TYP7(ra) {
  default: badarg: wta(ra, (char *)ARG1, s_array_imap);
  case tc7_vector: {
    SCM *ve = VELTS(ra);
    for (i = 0; i < LENGTH(ra); i++) {
      av[0] = MAKINUM(i);
      ve[i] = scm_cvapply(proc, 1L, av);
    }
    return UNSPECIFIED;
  }
  case tcs_uves:
    for (i = 0; i < LENGTH(ra); i++) {
      av[0] = MAKINUM(i);
      aset(ra, scm_cvapply(proc, 1L, auto_av), MAKINUM(i));
    }
    return UNSPECIFIED;
  case tc7_smob: ASRTGO(ARRAYP(ra), badarg);
    {
      int j, k, kmax = ARRAY_NDIM(ra) - 1;
      if (kmax < 0)
	return aset(ra, apply(proc, EOL, EOL), EOL);
      for (k = 0; k <= kmax; k++)
	indv[k] = ARRAY_DIMS(ra)[k].lbnd;
      k = kmax;
      do {
	if (k==kmax) {
	  indv[k] = ARRAY_DIMS(ra)[k].lbnd;
	  i = cind(ra, indv);
	  for (; indv[k] <= ARRAY_DIMS(ra)[k].ubnd; indv[k]++) {
	    for (j = kmax+1; j--;)
	      av[j] = MAKINUM(indv[j]);
	    aset(ARRAY_V(ra), scm_cvapply(proc, kmax+1L, av), MAKINUM(i));
	    i += ARRAY_DIMS(ra)[k].inc;
	  }
	  k--;
	  continue;
	}
	if (indv[k] < ARRAY_DIMS(ra)[k].ubnd) {
	  indv[k]++;
	  k++;
	  continue;
	}
	indv[k] = ARRAY_DIMS(ra)[k].lbnd - 1;
	k--;
      } while (k >= 0);
      return UNSPECIFIED;
    }
  }
}
