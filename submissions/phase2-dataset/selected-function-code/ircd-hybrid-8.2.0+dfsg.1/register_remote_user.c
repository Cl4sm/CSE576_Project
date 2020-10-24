void
register_remote_user(struct Client *source_p)
{
  const struct Client *target_p = NULL;

  if ((target_p = source_p->servptr) && target_p->from != source_p->from)
  {
    sendto_realops_flags(UMODE_DEBUG, L_ALL, SEND_NOTICE,
                         "Bad User [%s] :%s USER %s@%s %s, != %s[%s]",
                         source_p->from->name, source_p->name, source_p->username,
                         source_p->host, source_p->servptr->name,
                         target_p->name, target_p->from->name);
    sendto_one(source_p->from,
               ":%s KILL %s :%s (NICK from wrong direction (%s != %s))",
               me.id, source_p->id, me.name, source_p->servptr->name,
               target_p->from->name);

    AddFlag(source_p, FLAGS_KILLED);
    exit_client(source_p, "USER server wrong direction");
    return;
  }

  /*
   * If the nick has been introduced by a services server,
   * make it a service as well.
   */
  if (HasFlag(source_p->servptr, FLAGS_SERVICE))
    AddFlag(source_p, FLAGS_SERVICE);

  if (++Count.total > Count.max_tot)
    Count.max_tot = Count.total;

  SetClient(source_p);
  dlinkAdd(source_p, &source_p->lnode, &source_p->servptr->serv->client_list);
  dlinkAdd(source_p, &source_p->node, &global_client_list);
  add_user_host(source_p->username, source_p->host, 1);
  SetUserHost(source_p);

  if (HasFlag(source_p->servptr, FLAGS_EOB))
    sendto_realops_flags(UMODE_FARCONNECT, L_ALL, SEND_NOTICE,
                         "Client connecting at %s: %s (%s@%s) [%s] <%s>",
                         source_p->servptr->name,
                         source_p->name, source_p->username, source_p->host,
                         source_p->info, source_p->id);

  introduce_client(source_p);
}
