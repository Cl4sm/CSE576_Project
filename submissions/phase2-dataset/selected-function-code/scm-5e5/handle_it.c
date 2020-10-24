int handle_it(i)
     int i;
{
  SCM proc;
  char *name = errmsgs[i-WNA].s_response;
  if (errjmp_bad || errjmp_recursive)
    wta(UNDEFINED, (char *)i, ""); /* sends it to def_err_response */
  /* NEWCELL does not defer interrupts; so be careful to maintain the
     freelist integrity. */
  if (name) {
    int j;
    SCM n[2];	   /* GC-protect discarded cells (possibly being used
		      by interrupted code). */
    DEFER_INTS;
    /* Two cells are discarded because NEWCELL may have been
       interrupted between computing the right-hand-side of
          freelist = CDR(freelist)
       and assigning it to freelist. */
    for (j=0; j<2; j++) NEWCELL(n[j]); /* discard 2 possibly-used cells */
    CDR(n[1]) = EOL;		/* lose pointer to freelist */
    ALLOW_INTS;
    /* discarding was necessary here because intern() may do NEWCELL */
    proc = CDR(intern(name, (sizet)strlen(name)));
    if (NIMP(proc)) {	   /* Save environment stack, in case it moves
			      when applying proc.  Do an ecache gc to
			      protect contents of stack. */
      SCM estk, *estk_ptr, env, env_tmp;
      DEFER_INTS;
#ifndef NO_ENV_CACHE
      scm_egc();
#endif
      estk = scm_estk;
      estk_ptr = scm_estk_ptr;
      env = scm_env;
      env_tmp = scm_env_tmp;
      scm_estk = BOOL_F;
      scm_estk_reset(0);
      SCM_ESTK_PARENT(scm_estk) = estk;
      SCM_ESTK_PARENT_INDEX(scm_estk) = MAKINUM(estk_ptr - VELTS(estk));
      ALLOW_INTS;
      apply(proc, EOL, EOL);
      DEFER_INTS;
      scm_estk = estk;
      scm_estk_ptr = estk_ptr;
      scm_env = env;
      scm_env_tmp = env_tmp;
      scm_fill_freelist();
      ALLOW_INTS;
      return i;
    }
  }
  /* Ensure that freelist is not empty when returning from interrupt */
  DEFER_INTS;
  scm_fill_freelist();
  ALLOW_INTS;
  return errmsgs[i-WNA].parent_err;
}
