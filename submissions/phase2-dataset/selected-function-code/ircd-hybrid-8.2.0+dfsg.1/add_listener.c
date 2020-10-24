add_listener(int port, const char *vhost_ip, unsigned int flags)
{
  struct Listener *listener;
  struct irc_ssaddr vaddr;
  struct addrinfo hints, *res;
  char portname[PORTNAMELEN + 1];
  static short int pass = 0; /* if ipv6 and no address specified we need to
				have two listeners; one for each protocol. */

  /*
   * if no or invalid port in conf line, don't bother
   */
  if (!(port > 0 && port <= 0xFFFF))
    return;

  memset(&vaddr, 0, sizeof(vaddr));

  /* Set up the hints structure */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  /* Get us ready for a bind() and don't bother doing dns lookup */
  hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

  /* TBD: This makes no sense here at all. Needs to be reworked */

  snprintf(portname, sizeof(portname), "%d", port);
  getaddrinfo("::", portname, &hints, &res);
  vaddr.ss.ss_family = AF_INET6;
  assert(res != NULL);

  memcpy((struct sockaddr*)&vaddr, res->ai_addr, res->ai_addrlen);
  vaddr.ss_port = port;
  vaddr.ss_len = res->ai_addrlen;
  freeaddrinfo(res);
#ifdef MAKES_NO_SENSE
  {
    struct sockaddr_in *v4 = (struct sockaddr_in*) &vaddr;
    v4->sin_addr.s_addr = INADDR_ANY;
    vaddr.ss.ss_family = AF_INET;
    vaddr.ss_len = sizeof(struct sockaddr_in);
    v4->sin_port = htons(port);
  }
#endif

  snprintf(portname, PORTNAMELEN, "%d", port);

  if (!EmptyString(vhost_ip))
  {
    if (getaddrinfo(vhost_ip, portname, &hints, &res))
      return;

    assert(res);

    memcpy((struct sockaddr*)&vaddr, res->ai_addr, res->ai_addrlen);
    vaddr.ss_port = port;
    vaddr.ss_len = res->ai_addrlen;
    freeaddrinfo(res);
  }
  else if (pass == 0)
  {
    /* add the ipv4 listener if we havent already */
    pass = 1;
    add_listener(port, "0.0.0.0", flags);
  }

  pass = 0;

  if ((listener = find_listener(port, &vaddr)))
  {
    listener->flags = flags;

    if (listener->fd.flags.open)
      return;
  }
  else
  {
    listener = make_listener(port, &vaddr);
    dlinkAdd(listener, &listener->node, &listener_list);
    listener->flags = flags;
  }

  if (inetport(listener))
    listener->active = 1;
  else
    close_listener(listener);
}
