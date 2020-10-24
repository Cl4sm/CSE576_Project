     const char *what;
     SCM basecont;
{
  int j = num_protects;
  long oheap_cells = heap_cells;
  STACKITEM * stackbase = IMP(basecont) ? 0 : CONT(basecont)->stkbse;
#ifdef DEBUG_GMALLOC
  int err = check_frag_blocks();
  if (err) wta(MAKINUM(err), "malloc corrupted", what);
#endif
  gc_start(what);
  if (errjmp_bad) wta(UNDEFINED, s_recursive, s_gc);
  errjmp_bad = s_gc;
#ifdef NO_SYM_GC
  gc_mark(symhash);
#else
  /* By marking symhash first, we provide the best immunity from
     accidental references.  In order to accidentally protect a
     symbol, a pointer will have to point directly at the symbol (as
     opposed to the vector or bucket lists).  */
  mark_syms(symhash);
  /* mark_sym_values() can be called anytime after mark_syms.  */
  mark_sym_values(symhash);
#endif
  mark_subrs();
  egc_mark();
  if (stackbase) {
#ifdef __ia64__
    mark_regs_ia64(CONT(basecont));
#else
    jump_buf save_regs_gc_mark;
    FLUSH_REGISTER_WINDOWS;
    /* This assumes that all registers are saved into the jump_buf */
    setjump(save_regs_gc_mark);
    mark_locations((STACKITEM *) save_regs_gc_mark,
		   (sizet) (sizeof(STACKITEM) - 1 + sizeof save_regs_gc_mark) /
		   sizeof(STACKITEM));
    {
      /* stack_len is long rather than sizet in order to guarantee that
	 &stack_len is long aligned */
# ifdef STACK_GROWS_UP
#  ifdef nosve
      long stack_len = (STACKITEM *)(&stack_len) - stackbase;
#  else
      long stack_len = stack_size(stackbase);
#  endif
      mark_locations(stackbase, (sizet)stack_len);
# else
#  ifdef nosve
      long stack_len = stackbase - (STACKITEM *)(&stack_len);
#  else
      long stack_len = stack_size(stackbase);
#  endif
      mark_locations((stackbase - stack_len), (sizet)stack_len);
# endif
    }
#endif
  }
  while(j--)
    gc_mark(sys_protects[j]);
  mark_finalizers(&gc_finalizers, &gc_finalizers_pending);
#ifndef NO_SYM_GC
  sweep_symhash(symhash);
#endif
  gc_sweep(!stackbase);
  sweep_port_table();
  egc_sweep();
  estk_pool = EOL;
  errjmp_bad = (char *)0;
  gc_end();
  if (oheap_cells != heap_cells) {
    int grewp = heap_cells > oheap_cells;
    growth_mon(s_numheaps, (long)(hplim_ind/2), s_segs, grewp);
    growth_mon(s_heap, heap_cells, s_cells, grewp);
  }
  gc_hook_pending = !0;
  deferred_proc = process_signals;
}
