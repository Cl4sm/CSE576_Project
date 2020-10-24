exit_one_client(struct Client *source_p, const char *comment)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  assert(!IsMe(source_p));
  assert(source_p != &me);

  if (IsClient(source_p))
  {
    dlinkDelete(&source_p->lnode, &source_p->servptr->serv->client_list);
    dlinkDelete(&source_p->node, &global_client_list);

    /*
     * If a person is on a channel, send a QUIT notice
     * to every client (person) on the same channel (so
     * that the client can show the "**signoff" message).
     * (Note: The notice is to the local clients *only*)
     */
    sendto_common_channels_local(source_p, 0, 0, ":%s!%s@%s QUIT :%s",
                                 source_p->name, source_p->username,
                                 source_p->host, comment);
    DLINK_FOREACH_SAFE(ptr, ptr_next, source_p->channel.head)
      remove_user_from_channel(ptr->data);

    whowas_add_history(source_p, 0);
    whowas_off_history(source_p);

    watch_check_hash(source_p, RPL_LOGOFF);

    if (MyConnect(source_p))
    {
      /* Clean up invitefield */
      DLINK_FOREACH_SAFE(ptr, ptr_next, source_p->localClient->invited.head)
        del_invite(ptr->data, source_p);

      del_all_accepts(source_p);
    }
  }
  else if (IsServer(source_p))
  {
    dlinkDelete(&source_p->lnode, &source_p->servptr->serv->server_list);
    dlinkDelete(&source_p->node, &global_client_list);

    if ((ptr = dlinkFindDelete(&global_server_list, source_p)))
      free_dlink_node(ptr);
  }

  /* Remove source_p from the client lists */
  if (source_p->id[0])
    hash_del_id(source_p);
  if (source_p->name[0])
    hash_del_client(source_p);

  if (IsUserHostIp(source_p))
    delete_user_host(source_p->username, source_p->host, !MyConnect(source_p));

  update_client_exit_stats(source_p);

  /* Check to see if the client isn't already on the dead list */
  assert(dlinkFind(&dead_list, source_p) == NULL);

  /* add to dead client dlist */
  SetDead(source_p);
  dlinkAdd(source_p, make_dlink_node(), &dead_list);
}
