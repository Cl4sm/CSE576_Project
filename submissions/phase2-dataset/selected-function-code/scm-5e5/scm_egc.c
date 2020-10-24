{
  VERIFY_INTS("scm_egc", 0L);
/* We need to make sure there are enough cells available to migrate
   the entire environment cache, gc does not work properly during ecache gc */
  while (egc_need_gc()) {
    igc("ecache", rootcont);
    if ((gc_cells_collected < MIN_GC_YIELD) ||
	(heap_cells - cells_allocated <= scm_ecache_len) || IMP(freelist)) {
      alloc_some_heap();
      growth_mon("number of heaps", (long)(hplim_ind/2), "segments", !0);
      growth_mon(s_heap, heap_cells, s_cells, !0);
    }
  }
  if (errjmp_bad)
    wta(UNDEFINED, s_recursive, s_cache_gc);
  {
    SCM stkframe[2];
    long lcells = cells_allocated;
    sizet nstk = (scm_estk_ptr - VELTS(scm_estk) + SCM_ESTK_FRLEN);
    ASRTER(nstk<=LENGTH(scm_estk), UNDEFINED, "estk corrupted", s_cache_gc);
    scm_egc_start();
    stkframe[0] = scm_env;
    stkframe[1] = scm_env_tmp;
    egc_copy_roots();
    scm_clo_moved += cells_allocated - lcells;
    lcells = cells_allocated;
    egc_copy_locations(stkframe, sizeof(stkframe)/sizeof(SCM));
    egc_copy_stack(scm_estk, nstk);
    scm_env = stkframe[0];
    scm_env_tmp = stkframe[1];
    scm_stk_moved += cells_allocated - lcells;
    scm_ecache_index = scm_ecache_len;
    scm_env_work += scm_ecache_len;
    scm_egc_end();
  }
  errjmp_bad = (char *)0;
}
