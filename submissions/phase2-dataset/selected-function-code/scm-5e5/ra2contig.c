SCM ra2contig(ra, copy)
     SCM ra;
     int copy;
{
  SCM ret;
  long inc = 1;
  sizet k, len = 1;
  for (k = ARRAY_NDIM(ra); k--;)
    len *= ARRAY_DIMS(ra)[k].ubnd - ARRAY_DIMS(ra)[k].lbnd + 1;
  k = ARRAY_NDIM(ra);
  if (ARRAY_CONTP(ra) && ((0==k) || (1==ARRAY_DIMS(ra)[k-1].inc))) {
    if (tc7_Vbool != TYP7(ARRAY_V(ra)))
      return ra;
    if ((len==LENGTH(ARRAY_V(ra)) &&
	 0==ARRAY_BASE(ra) % LONG_BIT &&
	 0==len % LONG_BIT))
      return ra;
  }
  ret = make_ra(k);
  ARRAY_BASE(ret) = 0;
  while (k--) {
    ARRAY_DIMS(ret)[k].lbnd = ARRAY_DIMS(ra)[k].lbnd;
    ARRAY_DIMS(ret)[k].ubnd = ARRAY_DIMS(ra)[k].ubnd;
    ARRAY_DIMS(ret)[k].inc = inc;
    inc *= ARRAY_DIMS(ra)[k].ubnd - ARRAY_DIMS(ra)[k].lbnd + 1;
  }
  CAR(ret) |= ARRAY_CONTIGUOUS;
  ARRAY_V(ret) = make_uve(inc+0L, array_prot(ra));
  if (copy) array_copy(ret, ra);
  return ret;
}
