{
#ifdef WINSIGNALS
  init_sig1(INT_SIGNAL, SIGINT, win32_sigint);
#else
# ifdef SIGINT
  init_sig1(INT_SIGNAL, SIGINT, scmable_signal);
# endif
#endif
#ifdef SIGHUP
  init_sig1(HUP_SIGNAL, SIGHUP, scmable_signal);
#endif
#ifdef FLOATS
  init_sig1(FPE_SIGNAL, SIGFPE, err_signal);
#endif
#ifdef SIGBUS
  init_sig1(BUS_SIGNAL, SIGBUS, err_signal);
#endif
#ifdef SIGSEGV			/* AMIGA lacks! */
  init_sig1(SEGV_SIGNAL, SIGSEGV, err_signal);
#endif
#ifdef SIGALRM
  alarm(0);			/* kill any pending ALRM interrupts */
  init_sig1(ALRM_SIGNAL, SIGALRM, scmable_signal);
# ifdef SIGPROF
  init_sig1(PROF_SIGNAL, SIGPROF, scmable_signal);
# endif
# ifdef SIGVTALRM
  init_sig1(VTALRM_SIGNAL, SIGVTALRM, scmable_signal);
# endif
#endif
#ifdef SIGPIPE
  oldpipe = signal(SIGPIPE, SIG_IGN);
#endif
#ifdef ultrix
  siginterrupt(SIGINT, 1);
  siginterrupt(SIGALRM, 1);
  siginterrupt(SIGHUP, 1);
  siginterrupt(SIGPIPE, 1);
#endif /* ultrix */
}
