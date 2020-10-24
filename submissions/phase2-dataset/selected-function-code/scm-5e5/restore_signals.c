{
  int i;
#ifdef ultrix
  siginterrupt(SIGINT, 0);
  siginterrupt(SIGALRM, 0);
  siginterrupt(SIGHUP, 0);
  siginterrupt(SIGPIPE, 0);
#endif /* ultrix */
#ifdef SIGALRM
# ifndef SIGPROF
  alarm(0);			/* kill any pending ALRM interrupts */
# else
  i = sizeof(setitimer_tab)/sizeof(setitimer_tab[0]);
  while (i--)
    if (NIMP(setitimer_tab[i].sym))
      scm_setitimer(setitimer_tab[i].sym, BOOL_F, BOOL_F);
# endif
#endif
  i = NUM_SIGNALS;
  while (i--)
    if (sigdesc[i].signo)
      signal(sigdesc[i].signo, sigdesc[i].osig);
#ifdef SIGPIPE
  oldpipe = signal(SIGPIPE, SIG_IGN);
#endif
}
