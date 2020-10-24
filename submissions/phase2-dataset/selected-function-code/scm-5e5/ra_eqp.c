     SCM ra0, ras;
{
  SCM ra1 = CAR(ras), ra2 = CAR(CDR(ras));
  long n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra0)->lbnd + 1;
  sizet i0 = ARRAY_BASE(ra0), i1 = ARRAY_BASE(ra1), i2 = ARRAY_BASE(ra2);
  long inc0 = ARRAY_DIMS(ra0)->inc;
  long inc1 = ARRAY_DIMS(ra1)->inc;
  long inc2 = ARRAY_DIMS(ra2)->inc;
  ra0 = ARRAY_V(ra0);
  ra1 = ARRAY_V(ra1);
  ra2 = ARRAY_V(ra2);
  switch (TYP7(ra1)==TYP7(ra2) ? TYP7(ra1) : 0) {
  default: {
    SCM e1 = UNDEFINED, e2 = UNDEFINED;
    for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
      if (BVE_REF(ra0, i0))
	if (FALSEP(eqp(RVREF(ra1, i1, e1), RVREF(ra2, i2, e2))))
	  BVE_CLR(ra0, i0);
    break;
  }
  case tc7_VfixN32:
  case tc7_VfixZ32:
    for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
      if (BVE_REF(ra0, i0))
	if (VELTS(ra1)[i1] != VELTS(ra2)[i2]) BVE_CLR(ra0, i0);
    break;
# ifdef FLOATS
  case tc7_VfloR32:
    for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
      if (BVE_REF(ra0, i0))
	if (((float *)VELTS(ra1))[i1] != ((float *)VELTS(ra2))[i2])
	  BVE_CLR(ra0, i0);
    break;
  case tc7_VfloR64:
    for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
      if (BVE_REF(ra0, i0))
	if (((double *)VELTS(ra1))[i1] != ((double *)VELTS(ra2))[i2])
	  BVE_CLR(ra0, i0);
    break;
  case tc7_VfloC32:
    for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
      if (BVE_REF(ra0, i0))
	if (((float *)VELTS(ra1))[2*i1] != ((float *)VELTS(ra2))[2*i2] ||
	    ((float *)VELTS(ra1))[2*i1+1] != ((float *)VELTS(ra2))[2*i2+1])
	  BVE_CLR(ra0, i0);
    break;
  case tc7_VfloC64:
    for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
      if (BVE_REF(ra0, i0))
	if (((double *)VELTS(ra1))[2*i1] != ((double *)VELTS(ra2))[2*i2] ||
	    ((double *)VELTS(ra1))[2*i1+1] != ((double *)VELTS(ra2))[2*i2+1])
	  BVE_CLR(ra0, i0);
    break;
# endif /*FLOATS*/
  }
  return 1;
}
