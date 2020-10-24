serv_connect_callback(fde_t *fd, int status, void *data)
{
  struct Client *client_p = data;
  struct MaskItem *conf = NULL;

  /* First, make sure it's a real client! */
  assert(client_p);
  assert(&client_p->localClient->fd == fd);

  /* Next, for backward purposes, record the ip of the server */
  memcpy(&client_p->localClient->ip, &fd->connect.hostaddr,
         sizeof(struct irc_ssaddr));

  /* Check the status */
  if (status != COMM_OK)
  {
    /* We have an error, so report it and quit
     * Admins get to see any IP, mere opers don't *sigh*
     */
     if (ConfigServerHide.hide_server_ips)
       sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                            "Error connecting to %s: %s",
                            client_p->name, comm_errstr(status));
     else
       sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                            "Error connecting to %s[%s]: %s", client_p->name,
                            client_p->host, comm_errstr(status));

     sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                          "Error connecting to %s: %s",
                          client_p->name, comm_errstr(status));

     /* If a fd goes bad, call dead_link() the socket is no
      * longer valid for reading or writing.
      */
     dead_link_on_write(client_p, 0);
     return;
  }

  /* COMM_OK, so continue the connection procedure */
  /* Get the C/N lines */
  conf = find_conf_name(&client_p->localClient->confs,
                        client_p->name, CONF_SERVER);
  if (conf == NULL)
  {
    sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                         "Lost connect{} block for %s", get_client_name(client_p, HIDE_IP));
    sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                         "Lost connect{} block for %s", get_client_name(client_p, MASK_IP));

    exit_client(client_p, "Lost connect{} block");
    return;
  }

  /* Next, send the initial handshake */
  SetHandshake(client_p);

#ifdef HAVE_LIBCRYPTO
  if (IsConfSSL(conf))
  {
    ssl_connect_init(client_p, conf, fd);
    return;
  }
#endif

  sendto_one(client_p, "PASS %s TS %d %s", conf->spasswd, TS_CURRENT, me.id);

  send_capabilities(client_p, 0);

  sendto_one(client_p, "SERVER %s 1 :%s%s", me.name,
             ConfigServerHide.hidden ? "(H) " : "", me.info);

  /* If we've been marked dead because a send failed, just exit
   * here now and save everyone the trouble of us ever existing.
   */
  if (IsDead(client_p))
  {
      sendto_realops_flags(UMODE_ALL, L_ADMIN, SEND_NOTICE,
                           "%s[%s] went dead during handshake",
                           client_p->name,
                           client_p->host);
      sendto_realops_flags(UMODE_ALL, L_OPER, SEND_NOTICE,
                           "%s went dead during handshake", client_p->name);
      return;
  }

  /* don't move to serv_list yet -- we haven't sent a burst! */
  /* If we get here, we're ok, so lets start reading some data */
  comm_setselect(fd, COMM_SELECT_READ, read_packet, client_p, 0);
}
