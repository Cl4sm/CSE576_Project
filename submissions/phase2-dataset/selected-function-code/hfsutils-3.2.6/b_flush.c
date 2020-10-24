{
  bcache *cache = vol->cache;
  bucket *chain[HFS_CACHESZ];
  int i;

  if (cache == 0 || (vol->flags & HFS_VOL_READONLY))
    goto done;

  for (i = 0; i < HFS_CACHESZ; ++i)
    chain[i] = &cache->chain[i];

  if (flushbuckets(vol, chain, HFS_CACHESZ) == -1)
    goto fail;

done:
# ifdef DEBUG
  if (cache)
    b_showstats(cache);
# endif

  return 0;

fail:
  return -1;
}
