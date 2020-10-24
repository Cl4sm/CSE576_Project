check_server(const char *name, struct Client *client_p)
{
  dlink_node *ptr;
  struct MaskItem *conf        = NULL;
  struct MaskItem *server_conf = NULL;
  int error = -1;

  assert(client_p);

  /* loop through looking for all possible connect items that might work */
  DLINK_FOREACH(ptr, server_items.head)
  {
    conf = ptr->data;

    if (match(name, conf->name))
      continue;

    error = -3;

    /* XXX: Fix me for IPv6                    */
    /* XXX sockhost is the IPv4 ip as a string */
    if (!match(conf->host, client_p->host) ||
        !match(conf->host, client_p->sockhost))
    {
      error = -2;

      if (!match_conf_password(client_p->localClient->password, conf))
        return -2;

      if (!EmptyString(conf->certfp))
        if (EmptyString(client_p->certfp) || strcasecmp(client_p->certfp, conf->certfp))
          return -4;

      server_conf = conf;
    }
  }

  if (server_conf == NULL)
    return error;

  attach_conf(client_p, server_conf);


  if (server_conf)
  {
    struct sockaddr_in *v4;
    struct sockaddr_in6 *v6;

    switch (server_conf->aftype)
    {
      case AF_INET6:
        v6 = (struct sockaddr_in6 *)&server_conf->addr;

        if (IN6_IS_ADDR_UNSPECIFIED(&v6->sin6_addr))
          memcpy(&server_conf->addr, &client_p->localClient->ip, sizeof(struct irc_ssaddr));
        break;
      case AF_INET:
        v4 = (struct sockaddr_in *)&server_conf->addr;

        if (v4->sin_addr.s_addr == INADDR_NONE)
          memcpy(&server_conf->addr, &client_p->localClient->ip, sizeof(struct irc_ssaddr));
        break;
    }
  }

  return 0;
}
