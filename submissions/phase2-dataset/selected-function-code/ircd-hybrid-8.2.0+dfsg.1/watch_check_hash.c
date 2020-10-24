watch_check_hash(struct Client *client_p, const enum irc_numerics reply)
{
  struct Watch *anptr = NULL;
  dlink_node *ptr = NULL;

  assert(IsClient(client_p));

  if ((anptr = watch_find_hash(client_p->name)) == NULL)
    return;  /* This nick isn't on watch */

  /* Update the time of last change to item */
  anptr->lasttime = CurrentTime;

  /* Send notifies out to everybody on the list in header */
  DLINK_FOREACH(ptr, anptr->watched_by.head)
    sendto_one_numeric(ptr->data, &me, reply, client_p->name,
                       client_p->username, client_p->host,
                       anptr->lasttime, client_p->info);
}
