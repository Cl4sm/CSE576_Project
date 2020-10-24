start_auth_query(struct AuthRequest *auth)
{
  struct irc_ssaddr localaddr;
  socklen_t locallen = sizeof(struct irc_ssaddr);
  struct sockaddr_in6 *v6;

  /* open a socket of the same type as the client socket */
  if (comm_open(&auth->fd, auth->client->localClient->ip.ss.ss_family,
                SOCK_STREAM, 0, "ident") == -1)
  {
    report_error(L_ALL, "creating auth stream socket %s:%s",
                 get_client_name(auth->client, SHOW_IP), errno);
    ilog(LOG_TYPE_IRCD, "Unable to create auth socket for %s",
        get_client_name(auth->client, SHOW_IP));
    ++ServerStats.is_abad;
    return 0;
  }

  sendheader(auth->client, REPORT_DO_ID);

  /*
   * get the local address of the client and bind to that to
   * make the auth request.  This used to be done only for
   * ifdef VIRTUAL_HOST, but needs to be done for all clients
   * since the ident request must originate from that same address--
   * and machines with multiple IP addresses are common now
   */
  memset(&localaddr, 0, locallen);
  getsockname(auth->client->localClient->fd.fd, (struct sockaddr*)&localaddr,
      &locallen);

  remove_ipv6_mapping(&localaddr);
  v6 = (struct sockaddr_in6 *)&localaddr;
  v6->sin6_port = htons(0);
  localaddr.ss_port = htons(0);

  comm_connect_tcp(&auth->fd, auth->client->sockhost, RFC1413_PORT,
      (struct sockaddr *)&localaddr, localaddr.ss_len, auth_connect_callback,
      auth, auth->client->localClient->ip.ss.ss_family,
      GlobalSetOptions.ident_timeout);
  return 1; /* We suceed here for now */
}
