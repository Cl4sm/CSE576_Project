     int iverbose;
     int buf0stdin;
     long init_heap_size;
{
  STACKITEM i;
  if (2 <= iverbose) init_banner();
  if (!dumped) {
    init_types();
    init_tables();
    init_storage(&i, init_heap_size); /* CONT(rootcont)->stkbse gets set here */
  } else {
    /* The streams when the program was dumped need to be reset. */
    SETSTREAM(def_inp, stdin);
    SETSTREAM(def_outp, stdout);
    SETSTREAM(def_errp, stderr);
  }
  if (buf0stdin) SCM_PORTFLAGS(def_inp) |= BUF0;
  else SCM_PORTFLAGS(def_inp) &= ~BUF0;
  if (!dumped) {
    init_features();
    init_subrs();
    init_scl();
    init_unif();
    init_time();
    init_io();
    init_eval();		/* call to scm_evstr switches INTS discipline */
    init_debug();
    init_rope();
    init_repl(iverbose);
  }
  else reset_time();
#ifdef HAVE_DYNL
  /* init_dynl() must check dumped to avoid redefining subrs */
  init_dynl();
#endif
}
