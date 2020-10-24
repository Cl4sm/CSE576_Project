static int ramap_2o(ra0, proc, ras)
     SCM ra0, proc, ras;
{
  SCM ra1 = CAR(ras);
  SCM e1 = UNDEFINED;
  long n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra0)->lbnd + 1;
  sizet i0 = ARRAY_BASE(ra0), i1 = ARRAY_BASE(ra1);
  long inc0 = ARRAY_DIMS(ra0)->inc, inc1 = ARRAY_DIMS(ra1)->inc;
  ra0 = ARRAY_V(ra0);
  ra1 = ARRAY_V(ra1);
  ras = CDR(ras);
  if (NULLP(ras)) {
    if (tc7_vector==TYP7(ra0))
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	VELTS(ra0)[i0] = SUBRF(proc)(cvref(ra1, i1, UNDEFINED), UNDEFINED);
    else
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	aset(ra0, SUBRF(proc)(RVREF(ra1, i1, e1), UNDEFINED),
	     MAKINUM(i0));
  }
  else {
    SCM ra2 = CAR(ras);
    SCM e2 = UNDEFINED;
    sizet i2 = ARRAY_BASE(ra2);
    long inc2 = ARRAY_DIMS(ra2)->inc;
    ra2 = ARRAY_V(ra2);
    if (tc7_vector==TYP7(ra0))
      for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
	VELTS(ra0)[i0] =
	  SUBRF(proc)(cvref(ra1, i1, UNDEFINED), cvref(ra2, i2, UNDEFINED));
    else
      for (; n-- > 0; i0 += inc0, i1 += inc1, i2 += inc2)
	aset(ra0,
	     SUBRF(proc)(RVREF(ra1, i1, e1), RVREF(ra2, i2, e2)),
	     MAKINUM(i0));
  }
  return 1;
}
