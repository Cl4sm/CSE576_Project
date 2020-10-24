check_conf_klines(void)
{
  struct MaskItem *conf = NULL;
  dlink_node *ptr = NULL, *ptr_next = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, local_client_list.head)
  {
    struct Client *client_p = ptr->data;

    /* If a client is already being exited
     */
    if (IsDead(client_p) || !IsClient(client_p))
      continue;

    if ((conf = find_conf_by_address(NULL, &client_p->localClient->ip, CONF_DLINE,
                                     client_p->localClient->aftype, NULL, NULL, 1)))
    {
      conf_try_ban(client_p, conf);
      continue; /* and go examine next fd/client_p */
    }

    if (ConfigGeneral.glines)
    {
      if ((conf = find_conf_by_address(client_p->host, &client_p->localClient->ip,
                                       CONF_GLINE, client_p->localClient->aftype,
                                       client_p->username, NULL, 1)))
      {
        conf_try_ban(client_p, conf);
        /* and go examine next fd/client_p */
        continue;
      }
    }

    if ((conf = find_conf_by_address(client_p->host, &client_p->localClient->ip,
                                     CONF_KLINE, client_p->localClient->aftype,
                                     client_p->username, NULL, 1)))
    {
      conf_try_ban(client_p, conf);
      continue;
    }

    if ((conf = find_matching_name_conf(CONF_XLINE,  client_p->info,
                                        NULL, NULL, 0)))
    {
      conf_try_ban(client_p, conf);
      continue;
    }
  }

  /* also check the unknowns list for new dlines */
  DLINK_FOREACH_SAFE(ptr, ptr_next, unknown_list.head)
  {
    struct Client *client_p = ptr->data;

    if ((conf = find_conf_by_address(NULL, &client_p->localClient->ip, CONF_DLINE,
                                     client_p->localClient->aftype, NULL, NULL, 1)))
    {
      conf_try_ban(client_p, conf);
      continue; /* and go examine next fd/client_p */
    }
  }
}
