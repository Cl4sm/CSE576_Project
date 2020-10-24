int ramapc(cproc, data, ra0, lra, what)
     int (*cproc)();
     SCM data, ra0, lra;
     const char *what;
{
  SCM z, vra0, ra1, vra1;
  SCM lvra, *plvra;
  int k, kmax = (ARRAYP(ra0) ? ARRAY_NDIM(ra0) - 1 : 0);
  switch (ra_matchp(ra0, lra)) {
  default:
  case 0: wta(ra0, s_ra_mismatch, what);
  case 2: case 3: case 4:	/* Try unrolling arrays */
    if (kmax < 0) goto gencase;
    vra0 = (0==kmax ? ra0 : array_contents(ra0, UNDEFINED));
    if (IMP(vra0)) goto gencase;
    if (!ARRAYP(vra0)) {
      vra1 = make_ra(1);
      ARRAY_BASE(vra1) = 0;
      ARRAY_DIMS(vra1)->lbnd = 0;
      ARRAY_DIMS(vra1)->ubnd = LENGTH(vra0) - 1;
      ARRAY_DIMS(vra1)->inc = 1;
      ARRAY_V(vra1) = vra0;
      vra0 = vra1;
    }
    lvra = EOL;
    plvra = &lvra;
    for (z = lra; NIMP(z); z = CDR(z)) {
      vra1 = ra1 = (0==kmax ? CAR(z) : array_contents(CAR(z), UNDEFINED));
      if (FALSEP(ra1)) goto gencase;
      if (!ARRAYP(ra1)) {
	vra1 = make_ra(1);
	ARRAY_DIMS(vra1)->lbnd = ARRAY_DIMS(vra0)->lbnd;
	ARRAY_DIMS(vra1)->ubnd = ARRAY_DIMS(vra0)->ubnd;
	ARRAY_BASE(vra1) = 0;
	ARRAY_DIMS(vra1)->inc = 1;
	ARRAY_V(vra1) = ra1;
      }
      *plvra = cons(vra1, EOL);
      plvra = &CDR(*plvra);
    }
    return (UNBNDP(data) ? cproc(vra0, lvra) : cproc(vra0, data, lvra));
  case 1: gencase:		/* Have to loop over all dimensions. */
    {
      SCM hp_indv;
      long auto_indv[5];
      long *indv = &auto_indv[0];
      if (ARRAY_NDIM(ra0) >= 5) {
	scm_protect_temp(&hp_indv);
	hp_indv = make_uve(ARRAY_NDIM(ra0)+0L, MAKINUM(-32L));
	indv = (long *)VELTS(hp_indv);
      }
      vra0 = make_ra(1);
      if (ARRAYP(ra0)) {
	if (kmax < 0) {
	  ARRAY_DIMS(vra0)->lbnd = 0;
	  ARRAY_DIMS(vra0)->ubnd = 0;
	  ARRAY_DIMS(vra0)->inc = 1;
	}
	else {
	  ARRAY_DIMS(vra0)->lbnd = ARRAY_DIMS(ra0)[kmax].lbnd;
	  ARRAY_DIMS(vra0)->ubnd = ARRAY_DIMS(ra0)[kmax].ubnd;
	  ARRAY_DIMS(vra0)->inc = ARRAY_DIMS(ra0)[kmax].inc;
	}
	ARRAY_BASE(vra0) = ARRAY_BASE(ra0);
	ARRAY_V(vra0) = ARRAY_V(ra0);
      }
      else {
	ARRAY_DIMS(vra0)->lbnd = 0;
	ARRAY_DIMS(vra0)->ubnd = LENGTH(ra0) - 1;
	ARRAY_DIMS(vra0)->inc = 1;
	ARRAY_BASE(vra0) = 0;
	ARRAY_V(vra0) = ra0;
	ra0 = vra0;
      }
      lvra = EOL;
      plvra = &lvra;
      for (z = lra; NIMP(z); z = CDR(z)) {
	ra1 = CAR(z);
	vra1 = make_ra(1);
	ARRAY_DIMS(vra1)->lbnd = ARRAY_DIMS(vra0)->lbnd;
	ARRAY_DIMS(vra1)->ubnd = ARRAY_DIMS(vra0)->ubnd;
	if (ARRAYP(ra1)) {
	  if (kmax >= 0)
	    ARRAY_DIMS(vra1)->inc = ARRAY_DIMS(ra1)[kmax].inc;
	  ARRAY_V(vra1) = ARRAY_V(ra1);
	}
	else {
	  ARRAY_DIMS(vra1)->inc = 1;
	  ARRAY_V(vra1) = ra1;
	}
	*plvra = cons(vra1, EOL);
	plvra = &CDR(*plvra);
      }
      for (k = 0; k <= kmax; k++)
	indv[k] = ARRAY_DIMS(ra0)[k].lbnd;
      k = kmax;
      do {
	if (k==kmax) {
	  SCM y = lra;
	  ARRAY_BASE(vra0) = cind(ra0, indv);
	  for (z = lvra; NIMP(z); z = CDR(z), y = CDR(y))
	    ARRAY_BASE(CAR(z)) = cind(CAR(y), indv);
	  if (0==(UNBNDP(data) ? cproc(vra0, lvra) : cproc(vra0, data, lvra)))
	    return 0;
	  k--;
	  continue;
	}
	if (indv[k] < ARRAY_DIMS(ra0)[k].ubnd) {
	  indv[k]++;
	  k++;
	  continue;
	}
	indv[k] = ARRAY_DIMS(ra0)[k].lbnd - 1;
	k--;
      } while (k >= 0);
      return 1;
    }
  }
}
