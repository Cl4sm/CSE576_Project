void ignore_signals()
{
  int i = NUM_SIGNALS;
#ifdef ultrix
  siginterrupt(SIGINT, 0);
  siginterrupt(SIGALRM, 0);
  siginterrupt(SIGHUP, 0);
  siginterrupt(SIGPIPE, 0);
#endif /* ultrix */
  while (i--)
    if (sigdesc[i].signo)
      signal(sigdesc[i].signo, SIG_DFL);
  /* Some documentation claims that ALRMs are cleared accross forks.
     If this is not always true then the value returned by alarm(0)
     will have to be saved and unignore_signals() will have to
     reinstate it. */
  /* This code should be necessary only if the forked process calls
     alarm() without establishing a handler: */
#ifdef SIGALRM
  /* oldalrm = signal(SIGALRM, SIG_DFL); */
#endif
  /* These flushes are per warning in man page on fork(). */
  fflush(stdout);
  fflush(stderr);
#ifdef SIG_UNBLOCK
  {
    sigset_t set;
    sigfillset(&set);
    sigprocmask(SIG_UNBLOCK, &set, 0L);
  }
#endif
}
