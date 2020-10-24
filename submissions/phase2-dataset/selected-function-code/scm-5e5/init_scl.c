void init_scl()
{
  init_iprocs(subr1s, tc7_subr_1);
  init_iprocs(subr2os, tc7_subr_2o);
  init_iprocs(subr2s, tc7_subr_2);
  init_iprocs(asubrs, tc7_asubr);
  init_iprocs(rpsubrs, tc7_rpsubr);
#ifdef SICP
  add_feature("sicp");
#endif
#ifdef FLOATS
  init_iprocs((iproc *)cxrs, tc7_cxr);
# ifdef SINGLES
  NEWCELL(flo0);
  CAR(flo0) = tc_flo;
  FLO(flo0) = 0.0;
# else
  DEFER_INTS;
  flo0 = must_malloc_cell(1L*sizeof(double), (SCM)tc_dblr, "real");
  REAL(flo0) = 0.0;
  ALLOW_INTS;
# endif
# ifndef _MSC_VER
  DEFER_INTS;
  scm_narn = must_malloc_cell(2L*sizeof(double), (SCM)tc_dblc, "complex");
  REAL(scm_narn) = 0.0/0.0;
  IMAG(scm_narn) = 0.0/0.0;
  ALLOW_INTS;
# endif
# ifdef DBL_DIG
  dblprec = (DBL_DIG > 20) ? 20 : DBL_DIG;
# else
  {				/* determine floating point precision */
    double f = 0.1;
    volatile double fsum = 1.0+f;
    while (fsum != 1.0) {
      f /= 10.0;
      if (++dblprec > 20) break;
      safe_add_1(f, &fsum);
    }
    dblprec = dblprec-1;
  }
# endif /* DBL_DIG */
# ifdef DBL_MANT_DIG
  dbl_mant_dig = DBL_MANT_DIG;
# else
  {				/* means we #defined it. */
    volatile double fsum = 0.0;
    double eps = 1.0;
    int i = 0;
    while (fsum != 1.0) {
      eps = 0.5 * eps;
      safe_add_1(eps, &fsum);
      i++;
    }
    dbl_mant_dig = i;
  }
# endif /* DBL_MANT_DIG */
  max_dbl_int = pow(2.0, dbl_mant_dig - 1.0);
  max_dbl_int = max_dbl_int + (max_dbl_int - 1.0);
  dbl_eps = ldexp(1.0, - dbl_mant_dig);
  sysintern("double-float-mantissa-length", MAKINUM(dbl_mant_dig));
#endif
}
