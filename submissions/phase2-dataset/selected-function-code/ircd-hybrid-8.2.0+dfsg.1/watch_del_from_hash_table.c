watch_del_from_hash_table(const char *nick, struct Client *client_p)
{
  struct Watch *anptr = NULL;
  dlink_node *ptr = NULL;

  if ((anptr = watch_find_hash(nick)) == NULL)
    return;  /* No header found for that nick. i.e. it's not being watched */

  if ((ptr = dlinkFind(&anptr->watched_by, client_p)) == NULL)
    return;  /* This nick isn't being watched by client_p */

  dlinkDelete(ptr, &anptr->watched_by);
  free_dlink_node(ptr);

  if ((ptr = dlinkFindDelete(&client_p->localClient->watches, anptr)))
    free_dlink_node(ptr);

  /* In case this header is now empty of notices, remove it */
  if (anptr->watched_by.head == NULL)
  {
    assert(dlinkFind(&watchTable[strhash(anptr->nick)], anptr));
    dlinkDelete(&anptr->node, &watchTable[strhash(anptr->nick)]);
    mp_pool_release(anptr);
  }
}
