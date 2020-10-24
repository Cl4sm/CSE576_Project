{
  int i, n;
  unsigned long mask = 1L;
  /* printf("process_signals; output_deferred=%d\n", output_deferred); fflush(stdout); */
  if (output_deferred) {
    output_deferred = 0;
    /* if (NIMP(sys_errp) && OPOUTPORTP(sys_errp)) lfflush(sys_errp); */
  }
  for (n = 0; SIG_deferred && n < NUM_SIGNALS; n++) {
    if (SIG_deferred & mask) {
      i = n + SIGNAL_BASE;
      SIG_deferred &= ~mask;
      if (i != handle_it(i))
	wta(UNDEFINED, (char *)i, "");
    }
    mask <<= 1;
  }
  if (gc_hook_pending) {
    gc_hook_pending = 0;
    scm_gc_hook();
  }
  deferred_proc = 0;
}
