{
  SCM cont, estk, *from;
  CONTINUATION *ncont;
  sizet n;
  VERIFY_INTS("scm_make_cont", 0L);
  NEWCELL(cont);
  from = VELTS(scm_estk);
  n = scm_estk_ptr - from + SCM_ESTK_FRLEN;
#ifdef CHEAP_CONTINUATIONS
  estk = scm_estk;
#else
  from[1] = BOOL_F;		/* Can't write to parent stack */
  estk = must_malloc_cell((long)n*sizeof(SCM),
			  MAKE_LENGTH(n, tc7_vector), s_cont);
  {
    SCM *to = VELTS(estk);
    while(n--) to[n] = from[n];
  }
#endif
  ncont = make_continuation(CONT(rootcont));
  if (!ncont) wta(MAKINUM(-1), (char *)NALLOC, s_cont);
  ncont->other.parent = rootcont;
  SETCONT(cont, ncont);
  SETLENGTH(cont, ncont->length, tc7_contin);
  ncont->other.dynenv = dynwinds;
  ncont->other.stkframe[0] = scm_env;
  ncont->other.stkframe[1] = scm_env_tmp;
  ncont->other.estk = estk;
#ifdef CHEAP_CONTINUATIONS
  ncont->other.estk_ptr = scm_estk_ptr;
#else
  ncont->other.estk_ptr = (SCM *)0;
#endif
#ifndef RECKLESS
  ncont->other.stkframe[2] = scm_trace_env;
  ncont->other.stkframe[3] = scm_trace;
#endif
  return cont;
}
