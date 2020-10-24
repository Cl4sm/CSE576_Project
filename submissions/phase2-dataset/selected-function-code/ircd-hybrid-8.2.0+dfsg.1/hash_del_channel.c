void
hash_del_channel(struct Channel *chptr)
{
  unsigned int hashv = strhash(chptr->chname);
  struct Channel *tmp = channelTable[hashv];

  if (tmp)
  {
    if (tmp == chptr)
    {
      channelTable[hashv] = chptr->hnextch;
      chptr->hnextch = chptr;
    }
    else
    {
      while (tmp->hnextch != chptr)
        if ((tmp = tmp->hnextch) == NULL)
          return;

      tmp->hnextch = tmp->hnextch->hnextch;
      chptr->hnextch = chptr;
    }
  }
}
