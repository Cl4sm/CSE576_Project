     SCM args;
{
  SCM how_long, port1, port, ports, ans = EOL;
  int timeout, pos = ARG2;
  ASRTER(!NULLP(args), INUM0, WNA, s_wfi);
  how_long = CAR(args);
  args = CDR(args);
  if (NULLP(args)) port1 = cur_inp;
  else {
    port1 = CAR(args);
    args = CDR(args);
  }
  timeout = num2long(how_long, (char *)ARG1, s_wfi);
  ASRTER(timeout >= 0, how_long, ARG1, s_wfi);
  port = port1;
  ports = args;
  while (1) {
    ASRTER(NIMP(port) && OPINPORTP(port) && (BUF0 & SCM_PORTFLAGS(port)),
	   port, pos, s_wfi);
    if (CRDYP(port) || feof(STREAM(port))) timeout = 0;
    if (NULLP(ports)) break;
    if (ARG5 <= pos) pos = ARGn;
    else if (ARG1 < pos) pos = 1 + pos;
    port = CAR(ports);
    ports = CDR(ports);
  }
  {
#ifdef HAVE_SELECT
    fd_set ifds;
    struct timeval tv;
    int ret, fd_max = 0;

    tv.tv_sec = timeout;
    tv.tv_usec = 0;

    FD_ZERO(&ifds);
    port = port1;
    ports = args;
    while (1) {
      int fd = fileno(STREAM(port));
      FD_SET(fd, &ifds);
      if (fd_max < fd) fd_max = fd;

      if (NULLP(ports)) break;
      port = CAR(ports);
      ports = CDR(ports);
    }
    SYSCALL(ret = select(fd_max + 1, &ifds, (fd_set *)0L, (fd_set *)0L, &tv););
    ASRTER(ret>=0, MAKINUM(ret), "select error", s_wfi);

    port = port1;
    ports = args;
    while (1) {
      if (FD_ISSET(fileno(STREAM(port)), &ifds)
	  || CRDYP(port) || feof(STREAM(port)))
	ans = cons(port, ans);
      if (NULLP(ports)) break;
      port = CAR(ports);
      ports = CDR(ports);
    }
#else
    timet start = 0;
    time(&start);
    start = start + timeout;
    port = port1;
    ports = args;
    do {
      FILE *f = STREAM(port);
      if (feof(f)) ans = cons(port, ans);
      else {
# ifdef _WIN32
        if (fileno(f)==fileno(stdin) && (isatty(fileno(stdin))) && kbhit())
          ans = cons(port, ans);
# else
#  ifdef FIONREAD
        long remir;
        ioctl(fileno(f), FIONREAD, &remir);
        if (remir) ans = cons(port, ans);
#  else
        /* If we get here this is not going to work */
#  endif
# endif
          if (NULLP(ports)) break;
        port = CAR(ports);
        ports = CDR(ports);
      }
    } while (time((timet*)0L) < start);
#endif
    return NULLP(ans) ? BOOL_F : ans;
  }
}
