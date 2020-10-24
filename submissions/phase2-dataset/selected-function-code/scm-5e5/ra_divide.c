     SCM ra0, ras;
{
  long n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra0)->lbnd + 1;
  sizet i0 = ARRAY_BASE(ra0);
  long inc0 = ARRAY_DIMS(ra0)->inc;
  ra0 = ARRAY_V(ra0);
  if (NULLP(ras)) {
    switch TYP7(ra0) {
    default: {
      SCM e0 = UNDEFINED;
      for (; n-- > 0; i0 += inc0)
	aset(ra0, divide(RVREF(ra0, i0, e0), UNDEFINED), MAKINUM(i0));
      break;
    }
# ifdef FLOATS
    case tc7_VfloR32: {
      float *v0 = (float *)VELTS(ra0);
      for (; n-- > 0; i0 += inc0)
	v0[i0] = 1.0/v0[i0];
      break;
    }
    case tc7_VfloR64: {
      double *v0 = (double *)VELTS(ra0);
      for (; n-- > 0; i0 += inc0)
	v0[i0] = 1.0/v0[i0];
      break;
    }
    case tc7_VfloC32: {
      register double d;
      float (*v0)[2] = (float (*)[2])VELTS(ra0);
      for (; n-- > 0; i0 += inc0) {
	d = v0[i0][0]*v0[i0][0] + v0[i0][1]*v0[i0][1];
	v0[i0][0] /= d;
	v0[i0][1] /= -d;
      }
      break;
    }
    case tc7_VfloC64: {
      register double d;
      double (*v0)[2] = (double (*)[2])VELTS(ra0);
      for (; n-- > 0; i0 += inc0) {
	d = v0[i0][0]*v0[i0][0] + v0[i0][1]*v0[i0][1];
	v0[i0][0] /= d;
	v0[i0][1] /= -d;
      }
      break;
    }
# endif /* FLOATS */
    }
  }
  else {
    SCM ra1 = CAR(ras);
    sizet i1 = ARRAY_BASE(ra1);
    long inc1 = ARRAY_DIMS(ra1)->inc;
    ra1 = ARRAY_V(ra1);
    switch (TYP7(ra0)==TYP7(ra1) ? TYP7(ra0) : 0) {
    default: {
      SCM e0 = UNDEFINED, e1 = UNDEFINED;
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	aset(ra0, divide(RVREF(ra0, i0, e0), RVREF(ra1, i1, e1)), MAKINUM(i0));
      break;
    }
# ifdef FLOATS
    case tc7_VfloR32: {
      float *v0 = (float *)VELTS(ra0);
      float *v1 = (float *)VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1)
	      v0[i0] /= v1[i1]);
      break;
    }
    case tc7_VfloR64: {
      double *v0 = (double *)VELTS(ra0);
      double *v1 = (double *)VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1)
	      v0[i0] /= v1[i1]);
      break;
    }
    case tc7_VfloC32: {
      register double d, r;
      float (*v0)[2] = (float (*)[2])VELTS(ra0);
      float (*v1)[2] = (float (*)[2])VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1) {
	      d = v1[i1][0]*v1[i1][0] + v1[i1][1]*v1[i1][1];
	      r = (v0[i0][0]*v1[i1][0] + v0[i0][1]*v1[i1][1])/d;
	      v0[i0][1] = (v0[i0][1]*v1[i1][0] - v0[i0][0]*v1[i1][1])/d;
	      v0[i0][0] = r;
	    })
      break;
    }
    case tc7_VfloC64: {
      register double d, r;
      double (*v0)[2] = (double (*)[2])VELTS(ra0);
      double (*v1)[2] = (double (*)[2])VELTS(ra1);
      IVDEP(ra0 != ra1,
	    for (; n-- > 0; i0 += inc0, i1 += inc1) {
	      d = v1[i1][0]*v1[i1][0] + v1[i1][1]*v1[i1][1];
	      r = (v0[i0][0]*v1[i1][0] + v0[i0][1]*v1[i1][1])/d;
	      v0[i0][1] = (v0[i0][1]*v1[i1][0] - v0[i0][0]*v1[i1][1])/d;
	      v0[i0][0] = r;
	    })
      break;
    }
# endif /* FLOATS */
    }
  }
  return 1;
}
