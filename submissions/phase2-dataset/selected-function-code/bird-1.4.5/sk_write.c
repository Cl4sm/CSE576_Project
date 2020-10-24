int
sk_write(sock *s)
{
  switch (s->type)
  {
  case SK_TCP_ACTIVE:
    {
      sockaddr sa;
      sockaddr_fill(&sa, s->af, s->daddr, s->iface, s->dport);

      if (connect(s->fd, &sa.sa, SA_LEN(sa)) >= 0 || errno == EISCONN)
	sk_tcp_connected(s);
      else if (errno != EINTR && errno != EAGAIN && errno != EINPROGRESS)
	s->err_hook(s, errno);
      return 0;
    }

  default:
    if (s->ttx != s->tpos && sk_maybe_write(s) > 0)
    {
      if (s->tx_hook)
	s->tx_hook(s);
      return 1;
    }
    return 0;
  }
}
