static int
sk_setup(sock *s)
{
  int y = 1;
  int fd = s->fd;

  if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
    ERR("O_NONBLOCK");

  if (!s->af)
    return 0;

  if (ipa_nonzero(s->saddr) && !(s->flags & SKF_BIND))
    s->flags |= SKF_PKTINFO;

#ifdef CONFIG_USE_HDRINCL
  if (sk_is_ipv4(s) && (s->type == SK_IP) && (s->flags & SKF_PKTINFO))
  {
    s->flags &= ~SKF_PKTINFO;
    s->flags |= SKF_HDRINCL;
    if (setsockopt(fd, SOL_IP, IP_HDRINCL, &y, sizeof(y)) < 0)
      ERR("IP_HDRINCL");
  }
#endif

  if (s->iface)
  {
#ifdef SO_BINDTODEVICE
    struct ifreq ifr;
    strcpy(ifr.ifr_name, s->iface->name);
    if (setsockopt(s->fd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0)
      ERR("SO_BINDTODEVICE");
#endif

#ifdef CONFIG_UNIX_DONTROUTE
    if (setsockopt(s->fd, SOL_SOCKET, SO_DONTROUTE, &y, sizeof(y)) < 0)
      ERR("SO_DONTROUTE");
#endif
  }

  if (s->priority >= 0)
    if (sk_set_priority(s, s->priority) < 0)
      return -1;

  if (sk_is_ipv4(s))
  {
    if (s->flags & SKF_LADDR_RX)
      if (sk_request_cmsg4_pktinfo(s) < 0)
	return -1;

    if (s->flags & SKF_TTL_RX)
      if (sk_request_cmsg4_ttl(s) < 0)
	return -1;

    if ((s->type == SK_UDP) || (s->type == SK_IP))
      if (sk_disable_mtu_disc4(s) < 0)
	return -1;

    if (s->ttl >= 0)
      if (sk_set_ttl4(s, s->ttl) < 0)
	return -1;

    if (s->tos >= 0)
      if (sk_set_tos4(s, s->tos) < 0)
	return -1;
  }

  if (sk_is_ipv6(s))
  {
    if (s->flags & SKF_V6ONLY)
      if (setsockopt(fd, SOL_IPV6, IPV6_V6ONLY, &y, sizeof(y)) < 0)
	ERR("IPV6_V6ONLY");

    if (s->flags & SKF_LADDR_RX)
      if (sk_request_cmsg6_pktinfo(s) < 0)
	return -1;

    if (s->flags & SKF_TTL_RX)
      if (sk_request_cmsg6_ttl(s) < 0)
	return -1;

    if ((s->type == SK_UDP) || (s->type == SK_IP))
      if (sk_disable_mtu_disc6(s) < 0)
	return -1;

    if (s->ttl >= 0)
      if (sk_set_ttl6(s, s->ttl) < 0)
	return -1;

    if (s->tos >= 0)
      if (sk_set_tos6(s, s->tos) < 0)
	return -1;
  }

  return 0;
}
