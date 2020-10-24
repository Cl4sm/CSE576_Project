void init_ramap()
{
  init_raprocs(ra_rpsubrs);
  init_raprocs(ra_asubrs);
  init_iprocs(subr2os, tc7_subr_2o);
  /*  init_iprocs(subr2s, tc7_subr_2); */
  init_iprocs(lsubr2s, tc7_lsubr_2);
  make_subr(s_array_fill, tc7_subr_2, array_fill);
  make_subr(s_array_copy, tc7_subr_2, array_copy);
  make_subr(s_sarray_copy, tc7_subr_2, array_copy);
  make_subr(s_array_equalp, tc7_rpsubr, array_equal);
  smobs[0x0ff & (tc16_array>>8)].equalp = raequal;
  add_feature(s_array_for_each);
scm_ldstr("\n\
(define (array-indexes ra)\n\
  (let ((ra0 (apply make-array '#() (array-shape ra))))\n\
    (array-index-map! ra0 list)\n\
    ra0))\n\
(define (array-map prototype proc ra1 . ras)\n\
  (define nra (apply make-array prototype (array-shape ra1)))\n\
  (apply array-map! nra proc ra1 ras)\n\
  nra)\n\
");
}
