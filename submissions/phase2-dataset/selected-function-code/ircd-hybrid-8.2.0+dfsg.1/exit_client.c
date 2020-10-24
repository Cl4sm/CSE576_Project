void
exit_client(struct Client *source_p, const char *comment)
{
  dlink_node *m = NULL;

  assert(!IsMe(source_p));
  assert(source_p != &me);

  if (MyConnect(source_p))
  {
    /* DO NOT REMOVE. exit_client can be called twice after a failed
     * read/write.
     */
    if (IsClosing(source_p))
      return;

    SetClosing(source_p);

    if (HasFlag(source_p, FLAGS_IPHASH))
    {
      DelFlag(source_p, FLAGS_IPHASH);
      ipcache_remove_address(&source_p->localClient->ip);
    }

    delete_auth(&source_p->localClient->auth);

    /*
     * This source_p could have status of one of STAT_UNKNOWN, STAT_CONNECTING
     * STAT_HANDSHAKE or STAT_UNKNOWN
     * all of which are lumped together into unknown_list
     *
     * In all above cases IsRegistered() will not be true.
     */
    if (!IsRegistered(source_p))
    {
      assert(dlinkFind(&unknown_list, source_p));

      dlinkDelete(&source_p->localClient->lclient_node, &unknown_list);
    }
    else if (IsClient(source_p))
    {
      time_t on_for = CurrentTime - source_p->localClient->firsttime;
      assert(Count.local > 0);
      Count.local--;

      if (HasUMode(source_p, UMODE_OPER))
        if ((m = dlinkFindDelete(&oper_list, source_p)))
          free_dlink_node(m);

      assert(dlinkFind(&local_client_list, source_p));
      dlinkDelete(&source_p->localClient->lclient_node, &local_client_list);

      if (source_p->localClient->list_task)
        free_list_task(source_p);

      watch_del_watch_list(source_p);
      sendto_realops_flags(UMODE_CCONN, L_ALL, SEND_NOTICE,
                           "Client exiting: %s (%s@%s) [%s] [%s]",
                           source_p->name, source_p->username, source_p->host, comment,
                           ConfigGeneral.hide_spoof_ips && IsIPSpoof(source_p) ?
                           "255.255.255.255" : source_p->sockhost);
      ilog(LOG_TYPE_USER, "%s (%3u:%02u:%02u): %s!%s@%s %llu/%llu",
           myctime(source_p->localClient->firsttime), (unsigned int)(on_for / 3600),
           (unsigned int)((on_for % 3600)/60), (unsigned int)(on_for % 60),
           source_p->name, source_p->username, source_p->host,
           source_p->localClient->send.bytes>>10,
           source_p->localClient->recv.bytes>>10);
    }
    else if (IsServer(source_p))
    {
      assert(Count.myserver > 0);
      --Count.myserver;

      assert(dlinkFind(&local_server_list, source_p));
      dlinkDelete(&source_p->localClient->lclient_node, &local_server_list);
    }

    if (!IsDead(source_p))
    {
      if (IsServer(source_p))
      {
        if (!HasFlag(source_p, FLAGS_SQUIT))
        {
          /* for them, we are exiting the network */
          sendto_one(source_p, ":%s SQUIT %s :%s",
                     me.id, me.id, comment);
        }
      }

      sendto_one(source_p, "ERROR :Closing Link: %s (%s)",
                 source_p->host, comment);
    }

    close_connection(source_p);
  }
  else if (IsClient(source_p) && HasFlag(source_p->servptr, FLAGS_EOB))
    sendto_realops_flags(UMODE_FARCONNECT, L_ALL, SEND_NOTICE,
                         "Client exiting at %s: %s (%s@%s) [%s]",
                         source_p->servptr->name, source_p->name,
                         source_p->username, source_p->host, comment);

  if (IsServer(source_p))
  {
    char splitstr[HOSTLEN + HOSTLEN + 2] = "";

    /* This shouldn't ever happen */
    assert(source_p->serv && source_p->servptr);

    if (ConfigServerHide.hide_servers)
      /*
       * Set netsplit message to "*.net *.split" to still show
       * that its a split, but hide the servers splitting
       */
      strlcpy(splitstr, "*.net *.split", sizeof(splitstr));
    else
      snprintf(splitstr, sizeof(splitstr), "%s %s",
               source_p->servptr->name, source_p->name);

    /* Send SQUIT for source_p in every direction. source_p is already off of local_server_list here */
    if (!HasFlag(source_p, FLAGS_SQUIT))
      sendto_server(NULL, NOCAPS, NOCAPS, "SQUIT %s :%s", source_p->id, comment);

    /* Now exit the clients internally */
    recurse_remove_clients(source_p, splitstr);

    if (MyConnect(source_p))
    {
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "%s was connected for %d seconds.  %llu/%llu sendK/recvK.",
                           source_p->name, (int)(CurrentTime - source_p->localClient->firsttime),
                           source_p->localClient->send.bytes >> 10,
                           source_p->localClient->recv.bytes >> 10);
      ilog(LOG_TYPE_IRCD, "%s was connected for %d seconds.  %llu/%llu sendK/recvK.",
           source_p->name, (int)(CurrentTime - source_p->localClient->firsttime),
           source_p->localClient->send.bytes >> 10,
           source_p->localClient->recv.bytes >> 10);
    }
  }
  else if (IsClient(source_p) && !HasFlag(source_p, FLAGS_KILLED))
    sendto_server(source_p->from, NOCAPS, NOCAPS, ":%s QUIT :%s",
                  source_p->id, comment);

  /* The client *better* be off all of the lists */
  assert(dlinkFind(&unknown_list, source_p) == NULL);
  assert(dlinkFind(&local_client_list, source_p) == NULL);
  assert(dlinkFind(&local_server_list, source_p) == NULL);
  assert(dlinkFind(&oper_list, source_p) == NULL);

  exit_one_client(source_p, comment);
}
