int
hunt_server(struct Client *source_p, const char *command,
            const int server, const int parc, char *parv[])
{
  struct Client *target_p = NULL;
  dlink_node *ptr = NULL;

  /* Assume it's me, if no server */
  if (parc <= server || EmptyString(parv[server]))
    return HUNTED_ISME;

  if ((target_p = find_person(source_p, parv[server])) == NULL)
    target_p = hash_find_server(parv[server]);

  /*
   * These are to pickup matches that would cause the following
   * message to go in the wrong direction while doing quick fast
   * non-matching lookups.
   */
  if (target_p)
    if (target_p->from == source_p->from && !MyConnect(target_p))
      target_p = NULL;

  if (!target_p && has_wildcards(parv[server]))
  {
    DLINK_FOREACH(ptr, global_client_list.head)
    {
      struct Client *tmp = ptr->data;

      assert(IsMe(tmp) || IsServer(tmp) || IsClient(tmp));
      if (!match(parv[server], tmp->name))
      {
        if (tmp->from == source_p->from && !MyConnect(tmp))
          continue;

        target_p = ptr->data;
        break;
      }
    }
  }

  if (target_p)
  {
    assert(IsMe(target_p) || IsServer(target_p) || IsClient(target_p));
    if (IsMe(target_p) || MyClient(target_p))
      return HUNTED_ISME;

    parv[server] = target_p->id;
    sendto_one(target_p, command, source_p->id,
               parv[1], parv[2], parv[3], parv[4],
               parv[5], parv[6], parv[7], parv[8]);
    return HUNTED_PASS;
  }

  sendto_one_numeric(source_p, &me, ERR_NOSUCHSERVER, parv[server]);
  return HUNTED_NOSUCH;
}
