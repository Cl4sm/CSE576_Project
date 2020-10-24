static void
auth_connect_callback(fde_t *fd, int error, void *data)
{
  struct AuthRequest *auth = data;
  struct irc_ssaddr us;
  struct irc_ssaddr them;
  char authbuf[32];
  socklen_t ulen = sizeof(struct irc_ssaddr);
  socklen_t tlen = sizeof(struct irc_ssaddr);
  uint16_t uport, tport;
  struct sockaddr_in6 *v6;

  if (error != COMM_OK)
  {
    auth_error(auth);
    return;
  }

  if (getsockname(auth->client->localClient->fd.fd, (struct sockaddr *)&us, &ulen) ||
      getpeername(auth->client->localClient->fd.fd, (struct sockaddr *)&them, &tlen))
  {
    ilog(LOG_TYPE_IRCD, "auth get{sock,peer}name error for %s",
         get_client_name(auth->client, SHOW_IP));
    auth_error(auth);
    return;
  }

  v6 = (struct sockaddr_in6 *)&us;
  uport = ntohs(v6->sin6_port);
  v6 = (struct sockaddr_in6 *)&them;
  tport = ntohs(v6->sin6_port);
  remove_ipv6_mapping(&us);
  remove_ipv6_mapping(&them);

  snprintf(authbuf, sizeof(authbuf), "%u, %u\r\n", tport, uport);

  if (send(fd->fd, authbuf, strlen(authbuf), 0) == -1)
  {
    auth_error(auth);
    return;
  }

  comm_setselect(fd, COMM_SELECT_READ, read_auth_reply, auth, 0);
}
