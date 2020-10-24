int
sk_open(sock *s)
{
  int af = BIRD_AF;
  int fd = -1;
  int do_bind = 0;
  int bind_port = 0;
  ip_addr bind_addr = IPA_NONE;
  sockaddr sa;

  switch (s->type)
  {
  case SK_TCP_ACTIVE:
    s->ttx = "";			/* Force s->ttx != s->tpos */
    /* Fall thru */
  case SK_TCP_PASSIVE:
    fd = socket(af, SOCK_STREAM, IPPROTO_TCP);
    bind_port = s->sport;
    bind_addr = s->saddr;
    do_bind = bind_port || ipa_nonzero(bind_addr);
    break;
  
  case SK_UDP:
    fd = socket(af, SOCK_DGRAM, IPPROTO_UDP);
    bind_port = s->sport;
    bind_addr = (s->flags & SKF_BIND) ? s->saddr : IPA_NONE;
    do_bind = 1;
    break;

  case SK_IP:
    fd = socket(af, SOCK_RAW, s->dport);
    bind_port = 0;
    bind_addr = (s->flags & SKF_BIND) ? s->saddr : IPA_NONE;
    do_bind = ipa_nonzero(bind_addr);
    break;

  case SK_MAGIC:
    af = 0;
    fd = s->fd;
    break;

  default:
    bug("sk_open() called for invalid sock type %d", s->type);
  }

  if (fd < 0)
    ERR("socket");

  s->af = af;
  s->fd = fd;

  if (sk_setup(s) < 0)
    goto err;

  if (do_bind)
  {
    if (bind_port)
    {
      int y = 1;

      if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(y)) < 0)
	ERR2("SO_REUSEADDR");

#ifdef CONFIG_NO_IFACE_BIND
      /* Workaround missing ability to bind to an iface */
      if ((s->type == SK_UDP) && s->iface && ipa_zero(bind_addr))
      {
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &y, sizeof(y)) < 0)
	  ERR2("SO_REUSEPORT");
      }
#endif
    }

    sockaddr_fill(&sa, af, bind_addr, s->iface, bind_port);
    if (bind(fd, &sa.sa, SA_LEN(sa)) < 0)
      ERR2("bind");
  }

  if (s->password)
    if (sk_set_md5_auth(s, s->daddr, s->iface, s->password) < 0)
      goto err;

  switch (s->type)
  {
  case SK_TCP_ACTIVE:
    sockaddr_fill(&sa, af, s->daddr, s->iface, s->dport);
    if (connect(fd, &sa.sa, SA_LEN(sa)) >= 0)
      sk_tcp_connected(s);
    else if (errno != EINTR && errno != EAGAIN && errno != EINPROGRESS &&
	     errno != ECONNREFUSED && errno != EHOSTUNREACH && errno != ENETUNREACH)
      ERR2("connect");
    break;

  case SK_TCP_PASSIVE:
    if (listen(fd, 8) < 0)
      ERR2("listen");
    break;

  case SK_MAGIC:
    break;

  default:
    sk_alloc_bufs(s);
  }

  if (!(s->flags & SKF_THREAD))
    sk_insert(s);
  return 0;

err:
  close(fd);
  s->fd = -1;
  return -1;
}
