void
comm_connect_tcp(fde_t *fd, const char *host, unsigned short port, struct sockaddr *clocal,
                 int socklen, void (*callback)(fde_t *, int, void *), void *data,
                 int aftype, int timeout)
{
  struct addrinfo hints, *res;
  char portname[PORTNAMELEN + 1];

  assert(callback);
  fd->connect.callback = callback;
  fd->connect.data = data;

  fd->connect.hostaddr.ss.ss_family = aftype;
  fd->connect.hostaddr.ss_port = htons(port);

  /* Note that we're using a passed sockaddr here. This is because
   * generally you'll be bind()ing to a sockaddr grabbed from
   * getsockname(), so this makes things easier.
   * XXX If NULL is passed as local, we should later on bind() to the
   * virtual host IP, for completeness.
   *   -- adrian
   */
  if ((clocal != NULL) && (bind(fd->fd, clocal, socklen) < 0))
  {
    /* Failure, call the callback with COMM_ERR_BIND */
    comm_connect_callback(fd, COMM_ERR_BIND);
    /* ... and quit */
    return;
  }

  /* Next, if we have been given an IP, get the addr and skip the
   * DNS check (and head direct to comm_connect_tryconnect().
   */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

  snprintf(portname, sizeof(portname), "%d", port);

  if (getaddrinfo(host, portname, &hints, &res))
  {
    /* Send the DNS request, for the next level */
    if (aftype == AF_INET6)
      gethost_byname_type(comm_connect_dns_callback, fd, host, T_AAAA);
    else
      gethost_byname_type(comm_connect_dns_callback, fd, host, T_A);
  }
  else
  {
    /* We have a valid IP, so we just call tryconnect */
    /* Make sure we actually set the timeout here .. */
    assert(res != NULL);
    memcpy(&fd->connect.hostaddr, res->ai_addr, res->ai_addrlen);
    fd->connect.hostaddr.ss_len = res->ai_addrlen;
    fd->connect.hostaddr.ss.ss_family = res->ai_family;
    freeaddrinfo(res);
    comm_settimeout(fd, timeout*1000, comm_connect_timeout, NULL);
    comm_connect_tryconnect(fd, NULL);
  }
}
