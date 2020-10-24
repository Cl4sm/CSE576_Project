     SCM ra0, proc, ras;
{
  SCM e0 = UNDEFINED, e1 = UNDEFINED;
  long n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra0)->lbnd + 1;
  sizet i0 = ARRAY_BASE(ra0);
  long inc0 = ARRAY_DIMS(ra0)->inc;
  ra0 = ARRAY_V(ra0);
  if (NULLP(ras))
    for (; n-- > 0; i0 += inc0)
      aset(ra0, SUBRF(proc)(RVREF(ra0, i0, e0), UNDEFINED), MAKINUM(i0));
  else {
    SCM ra1 = CAR(ras);
    sizet i1 = ARRAY_BASE(ra1);
    long inc1 = ARRAY_DIMS(ra1)->inc;
    ra1 = ARRAY_V(ra1);
    for (; n-- > 0; i0 += inc0, i1 += inc1)
      aset(ra0, SUBRF(proc)(RVREF(ra0, i0, e0), RVREF(ra1, i1, e1)),
	   MAKINUM(i0));
  }
  return 1;
}
