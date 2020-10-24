add_connection(struct Listener *listener, struct irc_ssaddr *irn, int fd)
{
  struct Client *client_p = make_client(NULL);

  fd_open(&client_p->localClient->fd, fd, 1,
          (listener->flags & LISTENER_SSL) ?
          "Incoming SSL connection" : "Incoming connection");

  /*
   * copy address to 'sockhost' as a string, copy it to host too
   * so we have something valid to put into error messages...
   */
  memcpy(&client_p->localClient->ip, irn, sizeof(struct irc_ssaddr));

  getnameinfo((struct sockaddr *)&client_p->localClient->ip,
              client_p->localClient->ip.ss_len, client_p->sockhost,
              sizeof(client_p->sockhost), NULL, 0, NI_NUMERICHOST);
  client_p->localClient->aftype = client_p->localClient->ip.ss.ss_family;

#ifdef HAVE_LIBGEOIP
  /* XXX IPV6 SUPPORT XXX */
  if (irn->ss.ss_family == AF_INET && geoip_ctx)
  {
    const struct sockaddr_in *v4 = (const struct sockaddr_in *)&client_p->localClient->ip;
    client_p->localClient->country_id = GeoIP_id_by_ipnum(geoip_ctx, (unsigned long)ntohl(v4->sin_addr.s_addr));
  }
#endif

  if (client_p->sockhost[0] == ':' && client_p->sockhost[1] == ':')
  {
    strlcpy(client_p->host, "0", sizeof(client_p->host));
    strlcpy(client_p->host+1, client_p->sockhost, sizeof(client_p->host)-1);
    memmove(client_p->sockhost+1, client_p->sockhost, sizeof(client_p->sockhost)-1);
    client_p->sockhost[0] = '0';
  }
  else
    strlcpy(client_p->host, client_p->sockhost, sizeof(client_p->host));

  client_p->localClient->listener = listener;
  ++listener->ref_count;

#ifdef HAVE_LIBCRYPTO
  if (listener->flags & LISTENER_SSL)
  {
    if ((client_p->localClient->fd.ssl = SSL_new(ConfigServerInfo.server_ctx)) == NULL)
    {
      ilog(LOG_TYPE_IRCD, "SSL_new() ERROR! -- %s",
           ERR_error_string(ERR_get_error(), NULL));

      SetDead(client_p);
      exit_client(client_p, "SSL_new failed");
      return;
    }

    AddFlag(client_p, FLAGS_SSL);
    SSL_set_fd(client_p->localClient->fd.ssl, fd);
    ssl_handshake(NULL, client_p);
  }
  else
#endif
    start_auth(client_p);
}
