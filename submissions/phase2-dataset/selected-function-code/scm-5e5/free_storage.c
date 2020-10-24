{
  DEFER_INTS;
  loc_gc_hook = (SCM *)0;
  gc_start("free");
  errjmp_bad = "free_storage";
  cur_inp = BOOL_F; cur_outp = BOOL_F;
  cur_errp = tmp_errp; sys_errp = tmp_errp;
  gc_mark(def_inp);		/* don't want to close stdin */
  gc_mark(def_outp);		/* don't want to close stdout */
  gc_mark(def_errp);		/* don't want to close stderr */
  gc_sweep(0);
  rootcont = BOOL_F;
  while (hplim_ind) {		/* free heap segments */
    hplim_ind -= 2;
    {
      CELLPTR ptr = CELL_UP(hplims[hplim_ind]);
      sizet seg_cells = CELL_DN(hplims[hplim_ind+1]) - ptr;
      heap_cells -= seg_cells;
      free((char *)hplims[hplim_ind]);
      hplims[hplim_ind] = 0;
      growth_mon(s_heap, heap_cells, s_cells, 0); fflush(stderr);
    }}
  if (heap_cells) wta(MAKINUM(heap_cells), s_not_free, s_heap);
  if (hplim_ind) wta((SCM)MAKINUM(hplim_ind), s_not_free, s_hplims);
  /* Not all cells get freed (see gc_mark() calls above). */
  /* if (cells_allocated) wta(MAKINUM(cells_allocated), s_not_free, "cells"); */
  /* either there is a small memory leak or I am counting wrong. */
  must_free((char *)hplims, 0);
  /* if (mallocated) wta(MAKINUM(mallocated), s_not_free, "malloc"); */
  hplims = 0;
  scm_free_gra(&finals_gra);
  scm_free_gra(&smobs_gra);
  scm_free_gra(&subrs_gra);
  gc_end();
  ALLOW_INTS; /* A really bad idea, but printing does it anyway. */
  exit_report();
  lfflush(sys_errp);
  scm_free_gra(&ptobs_gra);
  lmallocated = mallocated = 0;
  /* Can't do gc_end() here because it uses ptobs which have been freed */
  fflush(stdout);		/* in lieu of close */
  fflush(stderr);		/* in lieu of close */
}
