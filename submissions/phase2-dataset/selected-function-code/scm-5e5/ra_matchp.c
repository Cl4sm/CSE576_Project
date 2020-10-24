int ra_matchp(ra0, ras)
     SCM ra0, ras;
{
  SCM ra1;
  array_dim dims;
  array_dim *s0 = &dims;
  array_dim *s1;
  sizet bas0 = 0;
  int i, ndim = 1;
  int exact = 2    /* 4 */;	/* Don't care about values >2 (yet?) */
  if (IMP(ra0)) return 0;
  switch TYP7(ra0) {
  default: return 0;
  case tc7_vector:
  case tcs_uves:
    s0->lbnd = 0;
    s0->inc = 1;
    s0->ubnd = (long)LENGTH(ra0) - 1;
    break;
  case tc7_smob:
    if (!ARRAYP(ra0)) return 0;
    ndim = ARRAY_NDIM(ra0);
    s0 = ARRAY_DIMS(ra0);
    bas0 = ARRAY_BASE(ra0);
    break;
  }
  while NIMP(ras) {
    ra1 = CAR(ras);
    switch (IMP(ra1) ? 0 : TYP7(ra1)) {
    default: scalar:
      CAR(ras) = sc2array(ra1, ra0, EOL); break;
    case tc7_vector:
    case tcs_uves:
      if (1 != ndim) return 0;
      switch (exact) {
      case 4: if (0 != bas0) exact = 3;
      case 3: if (1 != s0->inc) exact = 2;
      case 2: if ((0==s0->lbnd) && (s0->ubnd==LENGTH(ra1) - 1)) break;
	exact = 1;
      case 1: if (s0->lbnd < 0 || s0->ubnd >= LENGTH(ra1))
	if (s0->lbnd <= s0->ubnd) return 0;
      }
      break;
    case tc7_smob:
      if (!ARRAYP(ra1)) goto scalar;
      if (ndim != ARRAY_NDIM(ra1)) {
	if (0==ARRAY_NDIM(ra1))
	  goto scalar;
	else
	  return 0;
      }
      s1 = ARRAY_DIMS(ra1);
      if (bas0 != ARRAY_BASE(ra1)) exact = 3;
      for (i = 0; i < ndim; i++)
	switch (exact) {
	case 4: case 3:
	  if (s0[i].inc != s1[i].inc)
	    exact = 2;
	case 2:
	  if (s0[i].lbnd==s1[i].lbnd && s0[i].ubnd==s1[i].ubnd)
	    break;
	  exact = 1;
	default:
	  if (s0[i].lbnd < s1[i].lbnd || s0[i].ubnd > s1[i].ubnd)
	    if (s0[i].lbnd <= s0[i].ubnd) return 0;
	}
      break;
    }
    ras = CDR(ras);
  }
  return exact;
}
