hash_find_channel(const char *name)
{
  unsigned int hashv = strhash(name);
  struct Channel *chptr = NULL;

  if ((chptr = channelTable[hashv]))
  {
    if (irccmp(name, chptr->chname))
    {
      struct Channel *prev;

      while (prev = chptr, (chptr = chptr->hnextch))
      {
        if (!irccmp(name, chptr->chname))
        {
          prev->hnextch = chptr->hnextch;
          chptr->hnextch = channelTable[hashv];
          channelTable[hashv] = chptr;
          break;
        }
      }
    }
  }

  return chptr;
}
