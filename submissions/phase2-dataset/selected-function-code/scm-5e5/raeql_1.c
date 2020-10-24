static int raeql_1(ra0, as_equal, ra1)
     SCM ra0, as_equal, ra1;
{
  SCM e0 = UNDEFINED, e1 = UNDEFINED;
  sizet i0 = 0, i1 = 0;
  long inc0 = 1, inc1 = 1;
  sizet n = LENGTH(ra0);
  ra1 = CAR(ra1);
  if (ARRAYP(ra0)) {
    n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra0)->lbnd + 1;
    i0 = ARRAY_BASE(ra0);
    inc0 = ARRAY_DIMS(ra0)->inc;
    ra0 = ARRAY_V(ra0);
  }
  if (ARRAYP(ra1)) {
    i1 = ARRAY_BASE(ra1);
    inc1 = ARRAY_DIMS(ra1)->inc;
    ra1 = ARRAY_V(ra1);
  }
  switch TYP7(ra0) {
  case tc7_vector: default:
    for (; n--; i0+=inc0, i1+=inc1) {
      if (FALSEP(as_equal)) {
	if (FALSEP(array_equal(RVREF(ra0, i0, e0), RVREF(ra1, i1, e1))))
	  return 0;
      }
      else
	if (FALSEP(equal(RVREF(ra0, i0, e0), RVREF(ra1, i1, e1))))
	  return 0;
    }
    return 1;
  case tc7_string: {
    char *v0 = CHARS(ra0) + i0;
    char *v1 = CHARS(ra1) + i1;
    for (; n--; v0 += inc0, v1 += inc1)
      if (*v0 != *v1) return 0;
    return 1;
  }
  case tc7_Vbool:
    for (; n--; i0 += inc0, i1 += inc1)
      if (BVE_REF(ra0, i0) != BVE_REF(ra1, i1)) return 0;
    return 1;
  case tc7_VfixN32: case tc7_VfixZ32: {
    long *v0 = (long *)VELTS(ra0) + i0;
    long *v1 = (long *)VELTS(ra1) + i1;
    for (; n--; v0 += inc0, v1 += inc1)
      if (*v0 != *v1) return 0;
    return 1;
  }
# ifdef FLOATS
  case tc7_VfloR32: {
    float *v0 = (float *)VELTS(ra0) + i0;
    float *v1 = (float *)VELTS(ra1) + i1;
    for (; n--; v0 += inc0, v1 += inc1)
      if (*v0 != *v1) return 0;
    return 1;
  }
  case tc7_VfloR64: {
    double *v0 = (double *)VELTS(ra0) + i0;
    double *v1 = (double *)VELTS(ra1) + i1;
    for (; n--; v0 += inc0, v1 += inc1)
      if (*v0 != *v1) return 0;
    return 1;
  }
  case tc7_VfloC32: {
    float (*v0)[2]= (float (*)[2])VELTS(ra0) + i0;
    float (*v1)[2] = (float (*)[2])VELTS(ra1) + i1;
    for (; n--; v0 += inc0, v1 += inc1) {
      if ((*v0)[0] != (*v1)[0]) return 0;
      if ((*v0)[1] != (*v1)[1]) return 0;
    }
    return 1;
  }
  case tc7_VfloC64: {
    double (*v0)[2]= (double (*)[2])VELTS(ra0) + i0;
    double (*v1)[2] = (double (*)[2])VELTS(ra1) + i1;
    for (; n--; v0 += inc0, v1 += inc1) {
      if ((*v0)[0] != (*v1)[0]) return 0;
      if ((*v0)[1] != (*v1)[1]) return 0;
    }
    return 1;
  }
# endif /* FLOATS */
  }
}
