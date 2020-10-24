setup_signals(void)
{
  struct sigaction act;

  act.sa_flags = 0;
  act.sa_handler = SIG_IGN;

  sigemptyset(&act.sa_mask);
#ifdef SIGXFSZ
  sigaddset(&act.sa_mask, SIGXFSZ);
#endif
#ifdef SIGWINCH
  sigaddset(&act.sa_mask, SIGWINCH);
#endif
#ifdef SIGTRAP
  sigaddset(&act.sa_mask, SIGTRAP);
#endif
  sigaddset(&act.sa_mask, SIGPIPE);
  sigaddset(&act.sa_mask, SIGALRM);
  sigaddset(&act.sa_mask, SIGHUP);
  sigaddset(&act.sa_mask, SIGINT);
  sigaddset(&act.sa_mask, SIGTERM);
  sigaddset(&act.sa_mask, SIGUSR1);
  sigaddset(&act.sa_mask, SIGCHLD);

#ifdef SIGXFSZ
  sigaction(SIGXFSZ, &act, 0);
#endif
#ifdef SIGWINCH
  sigaction(SIGWINCH, &act, 0);
#endif
#ifdef SIGTRAP
  sigaction(SIGTRAP, &act, 0);
#endif
  sigaction(SIGPIPE, &act, 0);
  sigaction(SIGALRM, &act, 0);

  act.sa_handler = sighup_handler;
  sigaction(SIGHUP, &act, 0);

  act.sa_handler = sigint_handler;
  sigaction(SIGINT, &act, 0);

  act.sa_handler = sigterm_handler;
  sigaction(SIGTERM, &act, 0);

  act.sa_handler = sigusr1_handler;
  sigaction(SIGUSR1, &act, 0);

  act.sa_handler = sigchld_handler;
  sigaction(SIGCHLD, &act, 0);

  sigprocmask(SIG_UNBLOCK, &act.sa_mask, NULL);
}
