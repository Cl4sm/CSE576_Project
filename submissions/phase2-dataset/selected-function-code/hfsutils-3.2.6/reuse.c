static
int reuse(bcache *cache, bucket *b, unsigned long bnum)
{
  bucket *chain[HFS_BLOCKBUFSZ], *bptr;
  int i;

# ifdef DEBUG
  if (INUSE(b))
    fprintf(stderr, "BLOCK: CACHE reusing bucket containing "
	    "vol 0x%lx block %lu:%u\n",
	    (unsigned long) cache->vol, b->bnum, b->count);
# endif

  if (INUSE(b) && DIRTY(b))
    {
      /* flush most recently unused buckets */

      for (bptr = b, i = 0; i < HFS_BLOCKBUFSZ; ++i)
	{
	  chain[i] = bptr;
	  bptr = bptr->cprev;
	}

      if (flushbuckets(cache->vol, chain, HFS_BLOCKBUFSZ) == -1)
	goto fail;
    }

  b->flags &= ~HFS_BUCKET_INUSE;
  b->count  = 1;
  b->bnum   = bnum;

  return 0;

fail:
  return -1;
}
