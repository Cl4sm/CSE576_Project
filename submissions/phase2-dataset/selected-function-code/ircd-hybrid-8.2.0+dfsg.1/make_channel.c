make_channel(const char *chname)
{
  struct Channel *chptr = NULL;

  assert(!EmptyString(chname));

  chptr = mp_pool_get(channel_pool);

  /* Doesn't hurt to set it here */
  chptr->channelts = CurrentTime;
  chptr->last_join_time = CurrentTime;

  strlcpy(chptr->chname, chname, sizeof(chptr->chname));
  dlinkAdd(chptr, &chptr->node, &channel_list);

  hash_add_channel(chptr);

  return chptr;
}
