{
  CELLPTR ptr, *tmplims;
  sizet len = (2+hplim_ind)*sizeof(CELLPTR);
  ASRTGO(len==(2+hplim_ind)*sizeof(CELLPTR), badhplims);
  if (errjmp_bad) wta(UNDEFINED, "need larger initial", s_heap);
  tmplims = (CELLPTR *)must_realloc((char *)hplims,
				    len-2L*sizeof(CELLPTR), (long)len,
				    s_heap);
  /*  SYSCALL(tmplims = (CELLPTR *)realloc((char *)hplims, len);); */
  if (!tmplims)
badhplims:
    wta(UNDEFINED, s_nogrow, s_hplims);
  else hplims = tmplims;
  /* hplim_ind gets incremented in init_heap_seg() */
  if (expmem) {
    len = (sizet)(EXPHEAP(heap_cells)*sizeof(cell));
    if ((sizet)(EXPHEAP(heap_cells)*sizeof(cell)) != len) len = 0;
  }
  else len = HEAP_SEG_SIZE;
  while (len >= MIN_HEAP_SEG_SIZE) {
    SYSCALL(ptr = (CELLPTR) malloc(len););
    if (ptr) {
      init_heap_seg(ptr, len);
      return;
    }
    len /= 2;
  }
  wta(UNDEFINED, s_nogrow, s_heap);
}
