     SCM ra0, proc, ras;
{
  SCM ra1 = CAR(ras);
  SCM e1 = UNDEFINED;
  sizet i0 = ARRAY_BASE(ra0), i1 = ARRAY_BASE(ra1);
  long inc0 = ARRAY_DIMS(ra0)->inc, inc1 = ARRAY_DIMS(ra1)->inc;
  long n = ARRAY_DIMS(ra0)->ubnd - ARRAY_DIMS(ra1)->lbnd + 1;
  ra0 = ARRAY_V(ra0);
  ra1 = ARRAY_V(ra1);
  switch TYP7(ra0) {
  default: gencase:
    for (; n-- > 0; i0 += inc0, i1 += inc1) {
      e1 = cvref(ra1, i1, e1);
      aset(ra0, scm_cvapply(proc, 1L, &e1), MAKINUM(i0));
    }
    break;
# ifdef FLOATS
  case tc7_VfloR32: {
    float *dst = (float *)VELTS(ra0);
    switch TYP7(ra1) {
    default: goto gencase;
    case tc7_VfloR32:
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	dst[i0] = DSUBRF(proc)((double)((float *)VELTS(ra1))[i1]);
      break;
    case tc7_VfixN32:
    case tc7_VfixZ32:
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	dst[i0] = DSUBRF(proc)((double)VELTS(ra1)[i1]);
      break;
    }
    break;
  }
  case tc7_VfloR64: {
    double *dst = (double *)VELTS(ra0);
    switch TYP7(ra1) {
    default: goto gencase;
    case tc7_VfloR64:
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	dst[i0] = DSUBRF(proc)(((double *)VELTS(ra1))[i1]);
      break;
    case tc7_VfixN32:
    case tc7_VfixZ32:
      for (; n-- > 0; i0 += inc0, i1 += inc1)
	dst[i0] = DSUBRF(proc)((double)VELTS(ra1)[i1]);
      break;
    }
    break;
  }
# endif /* FLOATS */
  }
  return 1;
}
