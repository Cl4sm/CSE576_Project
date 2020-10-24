     SCM ra0, proc, lra;
{
  long narg = ilength(lra);
 tail:
#ifndef RECKLESS
    scm_arity_check(proc, narg, s_array_map);
#endif
  switch TYP7(proc) {
  default: gencase:
    ramapc(ramap, proc, ra0, lra, s_array_map);
    return UNSPECIFIED;
  case tc7_subr_1:
    ramapc(ramap_1, proc, ra0, lra, s_array_map);
    return UNSPECIFIED;
  case tc7_subr_2:
  case tc7_subr_2o:
    ramapc(ramap_2o, proc, ra0, lra, s_array_map);
    return UNSPECIFIED;
  case tc7_cxr: if (! SUBRF(proc)) goto gencase;
    ramapc(ramap_cxr, proc, ra0, lra, s_array_map);
    return UNSPECIFIED;
  case tc7_rpsubr: {
    ra_iproc *p;
    if (FALSEP(arrayp(ra0, BOOL_T))) goto gencase;
    array_fill(ra0, BOOL_T);
    for (p = ra_rpsubrs; p->name; p++)
      if (proc==p->sproc) {
	while (NNULLP(lra) && NNULLP(CDR(lra))) {
	  ramapc(p->vproc, UNDEFINED, ra0, lra, s_array_map);
	  lra = CDR(lra);
	}
	return UNSPECIFIED;
      }
    while (NNULLP(lra) && NNULLP(CDR(lra))) {
      ramapc(ramap_rp, proc, ra0, lra, s_array_map);
      lra = CDR(lra);
    }
    return UNSPECIFIED;
  }
  case tc7_asubr:
    if (NULLP(lra)) {
      SCM prot, fill = SUBRF(proc)(UNDEFINED, UNDEFINED);
      if (INUMP(fill)) {
	prot = array_prot(ra0);
# ifdef FLOATS
	if (NIMP(prot) && INEXP(prot))
	  fill = makdbl((double)INUM(fill), 0.0);
# endif
      }
      array_fill(ra0, fill);
    }
    else {
      SCM tail, ra1 = CAR(lra);
      SCM v0 = (NIMP(ra0) && ARRAYP(ra0) ? ARRAY_V(ra0) : ra0);
      ra_iproc *p;
      /* Check to see if order might matter.
	 This might be an argument for a separate
	 SERIAL-ARRAY-MAP! */
      if (v0==ra1 || (NIMP(ra1) && ARRAYP(ra1) && v0==ARRAY_V(ra1)))
	if (ra0 != ra1 || (ARRAYP(ra0) && !ARRAY_CONTP(ra0)))
	  goto gencase;
      for (tail = CDR(lra); NNULLP(tail); tail = CDR(tail)) {
	ra1 = CAR(tail);
	if (v0==ra1 || (NIMP(ra1) && ARRAYP(ra1) && v0==ARRAY_V(ra1)))
	  goto gencase;
      }
      for (p = ra_asubrs; p->name; p++)
	if (proc==p->sproc) {
	  if (ra0 != CAR(lra))
	    ramapc(ra_identity, UNDEFINED, ra0, cons(CAR(lra), EOL), s_array_map);
	  lra = CDR(lra);
	  while (1) {
	    ramapc(p->vproc, UNDEFINED, ra0, lra, s_array_map);
	    if (IMP(lra) || IMP(CDR(lra))) return UNSPECIFIED;
	    lra = CDR(lra);
	  }
	}
      ramapc(ramap_2o, proc, ra0, lra, s_array_map);
      lra = CDR(lra);
      if (NIMP(lra))
	for (lra = CDR(lra); NIMP(lra); lra = CDR(lra))
	  ramapc(ramap_a, proc, ra0, lra, s_array_map);
    }
    return UNSPECIFIED;
#if 1 /* def CCLO */
  case tc7_specfun:
    if (tc16_cclo==TYP16(proc)) {
      lra = cons(sc2array(proc, ra0, EOL), lra);
      proc = CCLO_SUBR(proc);
      narg++;
      goto tail;
    }
    goto gencase;
#endif
  }
}
