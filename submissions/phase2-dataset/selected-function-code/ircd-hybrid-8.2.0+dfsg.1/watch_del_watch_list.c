watch_del_watch_list(struct Client *client_p)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;
  dlink_node *tmp = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, client_p->localClient->watches.head)
  {
    struct Watch *anptr = ptr->data;

    assert(dlinkFind(&anptr->watched_by, client_p));

    if ((tmp = dlinkFindDelete(&anptr->watched_by, client_p)))
      free_dlink_node(tmp);

    /* If this leaves a header without notifies, remove it. */
    if (anptr->watched_by.head == NULL)
    {
      assert(dlinkFind(&watchTable[strhash(anptr->nick)], anptr));
      dlinkDelete(&anptr->node, &watchTable[strhash(anptr->nick)]);

      mp_pool_release(anptr);
    }

    dlinkDelete(ptr, &client_p->localClient->watches);
    free_dlink_node(ptr);
  }

  assert(client_p->localClient->watches.head == NULL);
  assert(client_p->localClient->watches.tail == NULL);
}
