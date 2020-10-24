add_nameserver(const char *arg)
{
  struct addrinfo hints, *res;

  /* Done max number of nameservers? */
  if (irc_nscount >= IRCD_MAXNS)
    return;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = PF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags    = AI_PASSIVE | AI_NUMERICHOST;

  if (getaddrinfo(arg, "domain", &hints, &res))
    return;

  if (res == NULL)
    return;

  memcpy(&irc_nsaddr_list[irc_nscount].ss, res->ai_addr, res->ai_addrlen);
  irc_nsaddr_list[irc_nscount++].ss_len = res->ai_addrlen;
  freeaddrinfo(res);
}
