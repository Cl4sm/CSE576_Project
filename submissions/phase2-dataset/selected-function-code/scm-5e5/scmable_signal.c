static SIGRETTYPE scmable_signal(sig)
     int sig;
{
  int oerr = errno;
  int i = NUM_SIGNALS;
  while (i--)
    if (sig == sigdesc[i].signo) break;
  ASRTER(i >= 0, MAKINUM(sig), s_unksig, "");
#ifdef WINSIGNALS
  if (SIGINT == sig) signal(sig, win32_sigint);
  else
#endif
    signal(sig, scmable_signal);
  if (ints_disabled) {
    deferred_proc = process_signals;
    SIG_deferred |= (1L << i);
  }
  else {
#ifdef SIG_UNBLOCK
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, sig);
    sigprocmask(SIG_UNBLOCK, &set, 0L);
#endif
    SIG_deferred &= ~(1L << i);
    i += SIGNAL_BASE;
    if (i != handle_it(i)) {
      errno = oerr;
      wta(UNDEFINED, (char *)i, "");
    }
  }
  errno = oerr;
}
