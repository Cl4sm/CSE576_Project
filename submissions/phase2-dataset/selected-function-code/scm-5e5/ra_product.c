int ra_product(ra0, ras)
     SCM ra0, ras;
{
  long n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra0)->lbnd + 1;
  sizet i0 = ARRAY_BASE(ra0);
  long inc0 = ARRAY_DIMS(ra0)->inc;
  ra0 = ARRAY_V(ra0);
  if (NNULLP(ras)) {
    SCM ra1 = CAR(ras);
    sizet i1 = ARRAY_BASE(ra1);
    long inc1 = ARRAY_DIMS(ra1)->inc;
    ra1 = ARRAY_V(ra1);
    switch (TYP7(ra0)==TYP7(ra1) ? TYP7(ra0) : 0) {
    ovflow: wta(ra0, (char *)OVFLOW, "*");
    default: {
      SCM e0 = UNDEFINED, e1 = UNDEFINED;
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	aset(ra0, product(RVREF(ra0, i0, e0), RVREF(ra1, i1, e1)),
	     MAKINUM(i0));
      break;
    }
    case tc7_VfixN32: {
      unsigned long r;
      unsigned long *v0 = (unsigned long *)VELTS(ra0);
      unsigned long *v1 = (unsigned long *)VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1) {
	      r = v0[i0] * v1[i1];
	      ASRTGO(0==v0[i0] || v1[i1]==r/v0[i0], ovflow);
	      v0[i0] = r;
	    } );
      break;
    }
    case tc7_VfixZ32: {
      long r, *v0 = VELTS(ra0), *v1 =VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1) {
	      r = v0[i0] * v1[i1];
	      ASRTGO(0==v0[i0] || v1[i1]==r/v0[i0], ovflow);
	      v0[i0] = r;
	    } );
      break;
    }
# ifdef FLOATS
    case tc7_VfloR32: {
      float *v0 = (float *)VELTS(ra0);
      float *v1 = (float *)VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1)
	      v0[i0] *= v1[i1]);
      break;
    }
    case tc7_VfloR64: {
      double *v0 = (double *)VELTS(ra0);
      double *v1 = (double *)VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1)
	      v0[i0] *= v1[i1]);
      break;
    }
    case tc7_VfloC32: {
      float (*v0)[2] = (float (*)[2])VELTS(ra0);
      register double r;
      float (*v1)[2] = (float (*)[2])VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1) {
	      r = v0[i0][0]*v1[i1][0] - v0[i0][1]*v1[i1][1];
	      v0[i0][1] = v0[i0][0]*v1[i1][1] + v0[i0][1]*v1[i1][0];
	      v0[i0][0] = r;
	    });
      break;
    }
    case tc7_VfloC64: {
      double (*v0)[2] = (double (*)[2])VELTS(ra0);
      register double r;
      double (*v1)[2] = (double (*)[2])VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1) {
	      r = v0[i0][0]*v1[i1][0] - v0[i0][1]*v1[i1][1];
	      v0[i0][1] = v0[i0][0]*v1[i1][1] + v0[i0][1]*v1[i1][0];
	      v0[i0][0] = r;
	    });
      break;
    }
# endif /* FLOATS */
    }
  }
  return 1;
}
