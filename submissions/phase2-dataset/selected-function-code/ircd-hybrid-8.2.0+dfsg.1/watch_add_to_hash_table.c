watch_add_to_hash_table(const char *nick, struct Client *client_p)
{
  struct Watch *anptr = NULL;
  dlink_node *ptr = NULL;

  /* If found NULL (no header for this nick), make one... */
  if ((anptr = watch_find_hash(nick)) == NULL)
  {
    anptr = mp_pool_get(watch_pool);

    anptr->lasttime = CurrentTime;
    strlcpy(anptr->nick, nick, sizeof(anptr->nick));

    dlinkAdd(anptr, &anptr->node, &watchTable[strhash(anptr->nick)]);
  }
  else
  {
    /* Is this client already on the watch-list? */
    ptr = dlinkFind(&anptr->watched_by, client_p);
  }

  if (ptr == NULL)
  {
    /* No it isn't, so add it in the bucket and client addint it */
    dlinkAdd(client_p, make_dlink_node(), &anptr->watched_by);
    dlinkAdd(anptr, make_dlink_node(), &client_p->localClient->watches);
  }
}
