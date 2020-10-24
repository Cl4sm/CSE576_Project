static int
sk_passive_connected(sock *s, int type)
{
  sockaddr loc_sa, rem_sa;
  int loc_sa_len = sizeof(loc_sa);
  int rem_sa_len = sizeof(rem_sa);

  int fd = accept(s->fd, ((type == SK_TCP) ? &rem_sa.sa : NULL), &rem_sa_len);
  if (fd < 0)
  {
    if ((errno != EINTR) && (errno != EAGAIN))
      s->err_hook(s, errno);
    return 0;
  }

  sock *t = sk_new(s->pool);
  t->type = type;
  t->fd = fd;
  t->af = s->af;
  t->ttl = s->ttl;
  t->tos = s->tos;
  t->rbsize = s->rbsize;
  t->tbsize = s->tbsize;

  if (type == SK_TCP)
  {
    if ((getsockname(fd, &loc_sa.sa, &loc_sa_len) < 0) ||
	(sockaddr_read(&loc_sa, s->af, &t->saddr, &t->iface, &t->sport) < 0))
      log(L_WARN "SOCK: Cannot get local IP address for TCP<");

    if (sockaddr_read(&rem_sa, s->af, &t->daddr, &t->iface, &t->dport) < 0)
      log(L_WARN "SOCK: Cannot get remote IP address for TCP<");
  }

  if (sk_setup(t) < 0)
  {
    /* FIXME: Call err_hook instead ? */
    log(L_ERR "SOCK: Incoming connection: %s%#m", t->err);

    /* FIXME: handle it better in rfree() */
    close(t->fd);	
    t->fd = -1;
    rfree(t);
    return 1;
  }

  sk_insert(t);
  sk_alloc_bufs(t);
  s->rx_hook(t, 0);
  return 1;
}
