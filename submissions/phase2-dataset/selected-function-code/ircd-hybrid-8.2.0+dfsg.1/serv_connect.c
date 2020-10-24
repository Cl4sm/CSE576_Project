serv_connect(struct MaskItem *conf, struct Client *by)
{
  struct Client *client_p = NULL;
  char buf[HOSTIPLEN + 1] = "";

  /* conversion structs */
  struct sockaddr_in *v4;

  /* Make sure conf is useful */
  assert(conf);

  getnameinfo((struct sockaddr *)&conf->addr, conf->addr.ss_len,
              buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);
  ilog(LOG_TYPE_IRCD, "Connect to %s[%s] @%s", conf->name, conf->host,
       buf);

  /* Still processing a DNS lookup? -> exit */
  if (conf->dns_pending)
  {
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Error connecting to %s: DNS lookup for connect{} in progress.",
                         conf->name);
    return 0;
  }

  if (conf->dns_failed)
  {
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Error connecting to %s: DNS lookup for connect{} failed.",
                         conf->name);
    return 0;
  }

  /* Make sure this server isn't already connected
   * Note: conf should ALWAYS be a valid C: line
   */
  if ((client_p = hash_find_server(conf->name)))
  {
    sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                         "Server %s already present from %s",
                         conf->name, get_client_name(client_p, SHOW_IP));
    sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                         "Server %s already present from %s",
                         conf->name, get_client_name(client_p, MASK_IP));
    if (by && IsClient(by) && !MyClient(by))
      sendto_one_notice(by, &me, ":Server %s already present from %s",
                        conf->name, get_client_name(client_p, MASK_IP));
    return 0;
  }

  /* Create a local client */
  client_p = make_client(NULL);

  /* Copy in the server, hostname, fd */
  strlcpy(client_p->name, conf->name, sizeof(client_p->name));
  strlcpy(client_p->host, conf->host, sizeof(client_p->host));

  /* We already converted the ip once, so lets use it - stu */
  strlcpy(client_p->sockhost, buf, sizeof(client_p->sockhost));

  /* create a socket for the server connection */
  if (comm_open(&client_p->localClient->fd, conf->addr.ss.ss_family, SOCK_STREAM, 0, NULL) < 0)
  {
    /* Eek, failure to create the socket */
    report_error(L_ALL, "opening stream socket to %s: %s", conf->name, errno);

    SetDead(client_p);
    exit_client(client_p, "Connection failed");
    return 0;
  }

  /* servernames are always guaranteed under HOSTLEN chars */
  fd_note(&client_p->localClient->fd, "Server: %s", conf->name);

  /* Attach config entries to client here rather than in
   * serv_connect_callback(). This to avoid null pointer references.
   */
  if (!attach_connect_block(client_p, conf->name, conf->host))
  {
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Host %s is not enabled for connecting: no connect{} block",
                         conf->name);
    if (by && IsClient(by) && !MyClient(by))
      sendto_one_notice(by, &me, ":Connect to host %s failed.", client_p->name);

    SetDead(client_p);
    exit_client(client_p, "Connection failed");
    return 0;
  }

  /* at this point we have a connection in progress and C/N lines
   * attached to the client, the socket info should be saved in the
   * client and it should either be resolved or have a valid address.
   *
   * The socket has been connected or connect is in progress.
   */
  make_server(client_p);

  if (by && IsClient(by))
    strlcpy(client_p->serv->by, by->name, sizeof(client_p->serv->by));
  else
    strlcpy(client_p->serv->by, "AutoConn.", sizeof(client_p->serv->by));

  SetConnecting(client_p);
  client_p->localClient->aftype = conf->aftype;

  /* Now, initiate the connection */
  /* XXX assume that a non 0 type means a specific bind address
   * for this connect.
   */
  switch (conf->aftype)
  {
    case AF_INET:
      v4 = (struct sockaddr_in*)&conf->bind;
      if (v4->sin_addr.s_addr)
      {
        struct irc_ssaddr ipn;
        memset(&ipn, 0, sizeof(struct irc_ssaddr));
        ipn.ss.ss_family = AF_INET;
        ipn.ss_port = 0;
        memcpy(&ipn, &conf->bind, sizeof(struct irc_ssaddr));
        comm_connect_tcp(&client_p->localClient->fd, conf->host, conf->port,
                         (struct sockaddr *)&ipn, ipn.ss_len,
                         serv_connect_callback, client_p, conf->aftype,
                         CONNECTTIMEOUT);
      }
      else if (ConfigServerInfo.specific_ipv4_vhost)
      {
        struct irc_ssaddr ipn;
        memset(&ipn, 0, sizeof(struct irc_ssaddr));
        ipn.ss.ss_family = AF_INET;
        ipn.ss_port = 0;
        memcpy(&ipn, &ConfigServerInfo.ip, sizeof(struct irc_ssaddr));
        comm_connect_tcp(&client_p->localClient->fd, conf->host, conf->port,
                         (struct sockaddr *)&ipn, ipn.ss_len,
                         serv_connect_callback, client_p, conf->aftype,
                         CONNECTTIMEOUT);
      }
      else
        comm_connect_tcp(&client_p->localClient->fd, conf->host, conf->port,
                         NULL, 0, serv_connect_callback, client_p, conf->aftype,
                         CONNECTTIMEOUT);
      break;
    case AF_INET6:
      {
        struct irc_ssaddr ipn;
        struct sockaddr_in6 *v6;
        struct sockaddr_in6 *v6conf;

        memset(&ipn, 0, sizeof(struct irc_ssaddr));
        v6conf = (struct sockaddr_in6 *)&conf->bind;
        v6 = (struct sockaddr_in6 *)&ipn;

        if (memcmp(&v6conf->sin6_addr, &v6->sin6_addr, sizeof(struct in6_addr)))
        {
          memcpy(&ipn, &conf->bind, sizeof(struct irc_ssaddr));
          ipn.ss.ss_family = AF_INET6;
          ipn.ss_port = 0;
          comm_connect_tcp(&client_p->localClient->fd,
                           conf->host, conf->port,
                           (struct sockaddr *)&ipn, ipn.ss_len,
                           serv_connect_callback, client_p,
                           conf->aftype, CONNECTTIMEOUT);
        }
        else if (ConfigServerInfo.specific_ipv6_vhost)
        {
          memcpy(&ipn, &ConfigServerInfo.ip6, sizeof(struct irc_ssaddr));
          ipn.ss.ss_family = AF_INET6;
          ipn.ss_port = 0;
          comm_connect_tcp(&client_p->localClient->fd,
                           conf->host, conf->port,
                           (struct sockaddr *)&ipn, ipn.ss_len,
                           serv_connect_callback, client_p,
                           conf->aftype, CONNECTTIMEOUT);
        }
        else
          comm_connect_tcp(&client_p->localClient->fd,
                           conf->host, conf->port,
                           NULL, 0, serv_connect_callback, client_p,
                           conf->aftype, CONNECTTIMEOUT);
      }
  }

  return 1;
}
