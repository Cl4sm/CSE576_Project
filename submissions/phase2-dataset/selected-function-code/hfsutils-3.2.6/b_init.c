int b_init(hfsvol *vol)
{
  bcache *cache;
  int i;

  ASSERT(vol->cache == 0);

  cache = ALLOC(bcache, 1);
  if (cache == 0)
    ERROR(ENOMEM, 0);

  vol->cache = cache;

  cache->vol    = vol;
  cache->tail   = &cache->chain[HFS_CACHESZ - 1];

  cache->hits   = 0;
  cache->misses = 0;

  for (i = 0; i < HFS_CACHESZ; ++i)
    {
      bucket *b = &cache->chain[i];

      b->flags = 0;
      b->count = 0;

      b->bnum  = 0;
      b->data  = &cache->pool[i];

      b->cnext = b + 1;
      b->cprev = b - 1;

      b->hnext = 0;
      b->hprev = 0;
    }

  cache->chain[0].cprev = cache->tail;
  cache->tail->cnext    = &cache->chain[0];

  for (i = 0; i < HFS_HASHSZ; ++i)
    cache->hash[i] = 0;

  return 0;

fail:
  return -1;
}
