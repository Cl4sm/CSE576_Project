int
sk_rx_ready(sock *s)
{
  fd_set rd, wr;
  struct timeval timo;
  int rv;

  FD_ZERO(&rd);
  FD_ZERO(&wr);
  FD_SET(s->fd, &rd);

  timo.tv_sec = 0;
  timo.tv_usec = 0;

 redo:
  rv = select(s->fd+1, &rd, &wr, NULL, &timo);
  
  if ((rv < 0) && (errno == EINTR || errno == EAGAIN))
    goto redo;

  return rv;
}
