void
lookup_confhost(struct MaskItem *conf)
{
  struct addrinfo hints, *res;

  /*
   * Do name lookup now on hostnames given and store the
   * ip numbers in conf structure.
   */
  memset(&hints, 0, sizeof(hints));

  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  /* Get us ready for a bind() and don't bother doing dns lookup */
  hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

  if (getaddrinfo(conf->host, NULL, &hints, &res))
  {
    conf_dns_lookup(conf);
    return;
  }

  assert(res);

  memcpy(&conf->addr, res->ai_addr, res->ai_addrlen);
  conf->addr.ss_len = res->ai_addrlen;
  conf->addr.ss.ss_family = res->ai_family;

  freeaddrinfo(res);
}
