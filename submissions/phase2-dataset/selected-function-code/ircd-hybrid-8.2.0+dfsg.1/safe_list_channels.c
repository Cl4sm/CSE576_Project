void
safe_list_channels(struct Client *source_p, int only_unmasked_channels)
{
  struct ListTask *lt = source_p->localClient->list_task;
  struct Channel *chptr = NULL;

  if (!only_unmasked_channels)
  {
    for (unsigned int i = lt->hash_index; i < HASHSIZE; ++i)
    {
      if (exceeding_sendq(source_p))
      {
        lt->hash_index = i;
        return;  /* Still more to do */
      }

      for (chptr = channelTable[i]; chptr; chptr = chptr->hnextch)
        list_one_channel(source_p, chptr);
    }
  }
  else
  {
    dlink_node *ptr = NULL;

    DLINK_FOREACH(ptr, lt->show_mask.head)
      if ((chptr = hash_find_channel(ptr->data)))
        list_one_channel(source_p, chptr);
  }

  free_list_task(source_p);
  sendto_one_numeric(source_p, &me, RPL_LISTEND);
}
