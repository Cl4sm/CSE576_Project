  int
tio_raw_readwait(tmout)
{
#ifdef FD_ZERO
  fd_set fdset;
  struct timeval tv_, *tv = &tv_;

  FD_ZERO(&fdset);
  FD_SET(0, &fdset);
  if (tmout < 0)
    tv = 0;
  else {
    tv->tv_sec = tmout / 1000;
    tv->tv_usec = tmout % 1000;
  }
  tio_flush();
  return select(1, &fdset, 0, 0, tv);
#else
#ifdef POLLIN
  struct pollfd fd;

  fd.fd = 0;
  fd.events = POLLIN;
  tio_flush();
  return poll(&fd, 1, tmout);
#else
  tio_flush();
  return 1;
    /* perhaps somebody can tell me a better way to handle this? */
#endif
#endif
}
