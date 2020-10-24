add_id(struct Client *client_p, struct Channel *chptr, char *banid, unsigned int type)
{
  dlink_list *list = NULL;
  dlink_node *ptr = NULL;
  struct Ban *banptr = NULL;
  size_t len = 0;
  char name[NICKLEN + 1] = "";
  char user[USERLEN + 1] = "";
  char host[HOSTLEN + 1] = "";
  struct split_nuh_item nuh;

  /* Don't let local clients overflow the b/e/I lists */
  if (MyClient(client_p))
  {
    unsigned int num_mask = dlink_list_length(&chptr->banlist) +
                            dlink_list_length(&chptr->exceptlist) +
                            dlink_list_length(&chptr->invexlist);

    if (num_mask >= ConfigChannel.max_bans)
    {
      sendto_one_numeric(client_p, &me, ERR_BANLISTFULL, chptr->chname, banid);
      return 0;
    }

    collapse(banid);
  }

  nuh.nuhmask  = check_string(banid);
  nuh.nickptr  = name;
  nuh.userptr  = user;
  nuh.hostptr  = host;

  nuh.nicksize = sizeof(name);
  nuh.usersize = sizeof(user);
  nuh.hostsize = sizeof(host);

  split_nuh(&nuh);

  /*
   * Re-assemble a new n!u@h and print it back to banid for sending
   * the mode to the channel.
   */
  len = sprintf(banid, "%s!%s@%s", name, user, host);

  switch (type)
  {
    case CHFL_BAN:
      list = &chptr->banlist;
      clear_ban_cache(chptr);
      break;
    case CHFL_EXCEPTION:
      list = &chptr->exceptlist;
      clear_ban_cache(chptr);
      break;
    case CHFL_INVEX:
      list = &chptr->invexlist;
      break;
    default:
      assert(0);
      return 0;
  }

  DLINK_FOREACH(ptr, list->head)
  {
    banptr = ptr->data;

    if (!irccmp(banptr->name, name) &&
        !irccmp(banptr->user, user) &&
        !irccmp(banptr->host, host))
      return 0;
  }

  banptr = mp_pool_get(ban_pool);
  banptr->name = xstrdup(name);
  banptr->user = xstrdup(user);
  banptr->host = xstrdup(host);
  banptr->when = CurrentTime;
  banptr->len = len - 2;  /* -2 for ! + @ */
  banptr->type = parse_netmask(host, &banptr->addr, &banptr->bits);

  if (IsClient(client_p))
  {
    banptr->who = MyCalloc(strlen(client_p->name) +
                           strlen(client_p->username) +
                           strlen(client_p->host) + 3);
    sprintf(banptr->who, "%s!%s@%s", client_p->name,
            client_p->username, client_p->host);
  }
  else if (IsHidden(client_p) || (IsServer(client_p) && ConfigServerHide.hide_servers))
    banptr->who = xstrdup(me.name);
  else
    banptr->who = xstrdup(client_p->name);

  dlinkAdd(banptr, &banptr->node, list);

  return 1;
}
