inetport(struct Listener *listener)
{
  struct irc_ssaddr lsin;
  socklen_t opt = 1;

  memset(&lsin, 0, sizeof(lsin));
  memcpy(&lsin, &listener->addr, sizeof(lsin));

  getnameinfo((struct sockaddr *)&lsin, lsin.ss_len, listener->name,
              sizeof(listener->name), NULL, 0, NI_NUMERICHOST);

  /*
   * At first, open a new socket
   */
  if (comm_open(&listener->fd, listener->addr.ss.ss_family, SOCK_STREAM, 0,
                "Listener socket") == -1)
  {
    report_error(L_ALL, "opening listener socket %s:%s",
                 get_listener_name(listener), errno);
    return 0;
  }

  if (setsockopt(listener->fd.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
  {
    report_error(L_ALL, "setting SO_REUSEADDR for listener %s:%s",
                 get_listener_name(listener), errno);
    fd_close(&listener->fd);
    return 0;
  }

  /*
   * Bind a port to listen for new connections if port is non-null,
   * else assume it is already open and try get something from it.
   */
  lsin.ss_port = htons(listener->port);

  if (bind(listener->fd.fd, (struct sockaddr *)&lsin, lsin.ss_len))
  {
    report_error(L_ALL, "binding listener socket %s:%s",
                 get_listener_name(listener), errno);
    fd_close(&listener->fd);
    return 0;
  }

  if (listen(listener->fd.fd, HYBRID_SOMAXCONN))
  {
    report_error(L_ALL, "listen failed for %s:%s",
                 get_listener_name(listener), errno);
    fd_close(&listener->fd);
    return 0;
  }

  /* Listen completion events are READ events .. */

  listener_accept_connection(&listener->fd, listener);
  return 1;
}
