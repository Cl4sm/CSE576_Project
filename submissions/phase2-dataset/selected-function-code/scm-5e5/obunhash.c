SCM obunhash(obj)
     SCM obj;
{
#ifdef BIGDIG
  if (NIMP(obj) && BIGP(obj)) {
    sizet i = NUMDIGS(obj);
    BIGDIG *ds = BDIGITS(obj);
    if (TYP16(obj)==tc16_bigpos) {
      obj = 0;
      while (i--) obj = BIGUP(obj) + ds[i];
    }
    else {
      obj = 0;
      while (i--) obj = BIGUP(obj) - ds[i];
    }
    obj <<= 1;
    goto comm;
  }
#endif
  ASRTER(INUMP(obj), obj, ARG1, s_obunhash);
  obj = SRS(obj, 1) & ~1L;
comm:
  if (IMP(obj)) return obj;
  if (NCELLP(obj)) return BOOL_F;
  {
    /* This code is adapted from mark_locations() in "sys.c" and
       scm_cell_p() in "rope.c", which means that changes to these
       routines must be coordinated. */
    register CELLPTR ptr = (CELLPTR)SCM2PTR(obj);
    register sizet i = 0, j = hplim_ind;
    do {
      if (PTR_GT(hplims[i++], ptr)) break;
      if (PTR_LE(hplims[--j], ptr)) break;
      if ((i != j)
	  && PTR_LE(hplims[i++], ptr)
	  && PTR_GT(hplims[--j], ptr)) continue;
      if (NFREEP(obj)) return obj;
      break;
    } while(i<j);
  }
  return BOOL_F;
}
