sk_read(sock *s)
{
  switch (s->type)
  {
  case SK_TCP_PASSIVE:
    return sk_passive_connected(s, SK_TCP);

  case SK_UNIX_PASSIVE:
    return sk_passive_connected(s, SK_UNIX);

  case SK_TCP:
  case SK_UNIX:
    {
      int c = read(s->fd, s->rpos, s->rbuf + s->rbsize - s->rpos);

      if (c < 0)
      {
	if (errno != EINTR && errno != EAGAIN)
	  s->err_hook(s, errno);
      }
      else if (!c)
	s->err_hook(s, 0);
      else
      {
	s->rpos += c;
	if (s->rx_hook(s, s->rpos - s->rbuf))
	{
	  /* We need to be careful since the socket could have been deleted by the hook */
	  if (current_sock == s)
	    s->rpos = s->rbuf;
	}
	return 1;
      }
      return 0;
    }

  case SK_MAGIC:
    return s->rx_hook(s, 0);

  default:
    {
      int e = sk_recvmsg(s);

      if (e < 0)
      {
	if (errno != EINTR && errno != EAGAIN)
	  s->err_hook(s, errno);
	return 0;
      }

      s->rpos = s->rbuf + e;
      s->rx_hook(s, e);
      return 1;
    }
  }
}
