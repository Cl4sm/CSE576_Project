void
register_local_user(struct Client *source_p)
{
  const char *id = NULL;
  const struct MaskItem *conf = NULL;

  assert(source_p == source_p->from);
  assert(MyConnect(source_p));
  assert(!source_p->localClient->registration);

  ClearCap(source_p, CAP_TS6);

  if (ConfigGeneral.ping_cookie)
  {
    if (!IsPingSent(source_p) && !source_p->localClient->random_ping)
    {
      do
        source_p->localClient->random_ping = genrand_int32();
      while (!source_p->localClient->random_ping);

      sendto_one(source_p, "PING :%u",
                 source_p->localClient->random_ping);
      SetPingSent(source_p);
      return;
    }

    if (!HasPingCookie(source_p))
      return;
  }

  source_p->localClient->last_privmsg = CurrentTime;
  /* Straight up the maximum rate of flooding... */
  source_p->localClient->allow_read = MAX_FLOOD_BURST;

  if (!check_client(source_p))
    return;

  if (!valid_hostname(source_p->host))
  {
    sendto_one_notice(source_p, &me, ":*** Notice -- You have an illegal "
                      "character in your hostname");
    strlcpy(source_p->host, source_p->sockhost,
            sizeof(source_p->host));
  }

  conf = source_p->localClient->confs.head->data;

  if (!IsGotId(source_p))
  {
    char username[USERLEN + 1] = "";
    const char *p = username;
    unsigned int i = 0;

    if (IsNeedIdentd(conf))
    {
      ++ServerStats.is_ref;
      sendto_one_notice(source_p, &me, ":*** Notice -- You need to install "
                        "identd to use this server");
      exit_client(source_p, "Install identd");
      return;
    }

    strlcpy(username, source_p->username, sizeof(username));

    if (!IsNoTilde(conf))
      source_p->username[i++] = '~';

    for (; *p && i < USERLEN; ++p)
      source_p->username[i++] = *p;

    source_p->username[i] = '\0';
  }

  /* Password check */
  if (!EmptyString(conf->passwd))
  {
    if (!match_conf_password(source_p->localClient->password, conf))
    {
      ++ServerStats.is_ref;

      sendto_one_numeric(source_p, &me, ERR_PASSWDMISMATCH);
      exit_client(source_p, "Bad Password");
      return;
    }
  }

  /*
   * Don't free source_p->localClient->password here - it can be required
   * by masked /stats I if there are auth{} blocks with need_password = no;
   * --adx
   */

  /*
   * Report if user has &^>= etc. and set flags as needed in source_p
   */
  report_and_set_user_flags(source_p, conf);

  if (IsDead(source_p))
    return;

  /*
   * Limit clients -
   * We want to be able to have servers and F-line clients
   * connect, so save room for "buffer" connections.
   * Smaller servers may want to decrease this, and it should
   * probably be just a percentage of the MAXCLIENTS...
   *   -Taner
   */
  if ((Count.local >= ConfigServerInfo.max_clients + MAX_BUFFER) ||
      (Count.local >= ConfigServerInfo.max_clients && !IsExemptLimits(source_p)))
  {
    sendto_realops_flags(UMODE_FULL, L_ALL, SEND_NOTICE,
                         "Too many clients, rejecting %s[%s].",
                         source_p->name, source_p->host);
    ++ServerStats.is_ref;
    exit_client(source_p, "Sorry, server is full - try later");
    return;
  }

  if (!valid_username(source_p->username, 1))
  {
    char buf[IRCD_BUFSIZE] = "";

    sendto_realops_flags(UMODE_REJ, L_ALL, SEND_NOTICE,
                         "Invalid username: %s (%s@%s)",
                         source_p->name, source_p->username, source_p->host);
    ++ServerStats.is_ref;
    snprintf(buf, sizeof(buf), "Invalid username [%s]", source_p->username);
    exit_client(source_p, buf);
    return;
  }

  if (check_xline(source_p))
    return;

  while (hash_find_id((id = uid_get())))
    ;

  strlcpy(source_p->id, id, sizeof(source_p->id));
  hash_add_id(source_p);

  sendto_realops_flags(UMODE_CCONN, L_ALL, SEND_NOTICE,
                       "Client connecting: %s (%s@%s) [%s] {%s} [%s] <%s>",
                       source_p->name, source_p->username, source_p->host,
                       ConfigGeneral.hide_spoof_ips && IsIPSpoof(source_p) ?
                       "255.255.255.255" : source_p->sockhost,
                       get_client_class(&source_p->localClient->confs),
                       source_p->info, source_p->id);

  if (ConfigGeneral.invisible_on_connect)
  {
    AddUMode(source_p, UMODE_INVISIBLE);
    ++Count.invisi;
  }

  if (++Count.local > Count.max_loc)
  {
    Count.max_loc = Count.local;

    if (!(Count.max_loc % 10))
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "New Max Local Clients: %d",
                           Count.max_loc);
  }

  if (++Count.total > Count.max_tot)
    Count.max_tot = Count.total;
  ++Count.totalrestartcount;

  assert(source_p->servptr == &me);
  SetClient(source_p);
  dlinkAdd(source_p, &source_p->lnode, &source_p->servptr->serv->client_list);
  dlinkAdd(source_p, &source_p->node, &global_client_list);

  assert(dlinkFind(&unknown_list, source_p));

  dlink_move_node(&source_p->localClient->lclient_node,
                  &unknown_list, &local_client_list);

  user_welcome(source_p);
  add_user_host(source_p->username, source_p->host, 0);
  SetUserHost(source_p);

  introduce_client(source_p);
}
