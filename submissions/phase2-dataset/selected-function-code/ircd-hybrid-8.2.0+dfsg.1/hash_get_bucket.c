void *
hash_get_bucket(int type, unsigned int hashv)
{
  assert(hashv < HASHSIZE);
  if (hashv >= HASHSIZE)
      return NULL;

  switch (type)
  {
    case HASH_TYPE_ID:
      return idTable[hashv];
      break;
    case HASH_TYPE_CHANNEL:
      return channelTable[hashv];
      break;
    case HASH_TYPE_CLIENT:
      return clientTable[hashv];
      break;
    case HASH_TYPE_USERHOST:
      return userhostTable[hashv];
      break;
    default:
      assert(0);
  }

  return NULL;
}
