sk_maybe_write(sock *s)
{
  int e;

  switch (s->type)
  {
  case SK_TCP:
  case SK_MAGIC:
  case SK_UNIX:
    while (s->ttx != s->tpos)
    {
      e = write(s->fd, s->ttx, s->tpos - s->ttx);

      if (e < 0)
      {
	if (errno != EINTR && errno != EAGAIN)
	{
	  reset_tx_buffer(s);
	  /* EPIPE is just a connection close notification during TX */
	  s->err_hook(s, (errno != EPIPE) ? errno : 0);
	  return -1;
	}
	return 0;
      }
      s->ttx += e;
    }
    reset_tx_buffer(s);
    return 1;

  case SK_UDP:
  case SK_IP:
    {
      if (s->tbuf == s->tpos)
	return 1;

      e = sk_sendmsg(s);

      if (e < 0)
      {
	if (errno != EINTR && errno != EAGAIN)
	{
	  reset_tx_buffer(s);
	  s->err_hook(s, errno);
	  return -1;
	}

	if (!s->tx_hook)
	  reset_tx_buffer(s);
	return 0;
      }
      reset_tx_buffer(s);
      return 1;
    }
  default:
    bug("sk_maybe_write: unknown socket type %d", s->type);
  }
}
