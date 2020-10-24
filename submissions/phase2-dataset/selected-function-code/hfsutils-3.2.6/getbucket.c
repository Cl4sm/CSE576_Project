static
bucket *getbucket(bcache *cache, unsigned long bnum, int fill)
{
  bucket **hslot, *b, *p, *bptr,
    *chain[HFS_BLOCKBUFSZ], **slots[HFS_BLOCKBUFSZ];

  b = findbucket(cache, bnum, &hslot);

  if (b)
    {
      /* cache hit; move towards head of cache chain */

      ++cache->hits;

      if (++b->count > b->cprev->count &&
	  b != cache->tail->cnext)
	{
	  p = b->cprev;

	  p->cprev->cnext = b;
	  b->cnext->cprev = p;

	  p->cnext = b->cnext;
	  b->cprev = p->cprev;

	  p->cprev = b;
	  b->cnext = p;

	  if (cache->tail == b)
	    cache->tail = p;
	}
    }
  else
    {
      /* cache miss; reuse least-used cache bucket */

      ++cache->misses;

      b = cache->tail;

      if (reuse(cache, b, bnum) == -1)
	goto fail;

      if (fill)
	{
	  unsigned int len = 0;

	  chain[len]   = b;
	  slots[len++] = hslot;

	  for (bptr = b->cprev;
	       len < (HFS_BLOCKBUFSZ >> 1) && ++bnum < cache->vol->vlen;
	       bptr = bptr->cprev)
	    {
	      if (findbucket(cache, bnum, &hslot))
		break;

	      if (reuse(cache, bptr, bnum) == -1)
		goto fail;

	      chain[len]   = bptr;
	      slots[len++] = hslot;
	    }

	  if (fillbuckets(cache->vol, chain, len) == -1)
	    goto fail;

	  while (--len)
	    {
	      cplace(cache, chain[len]);
	      hplace(slots[len], chain[len]);
	    }

	  hslot = slots[0];
	}

      /* move bucket to appropriate place in chain */

      cplace(cache, b);
    }

  /* insert at front of hash chain */

  hplace(hslot, b);

  return b;

fail:
  return 0;
}
