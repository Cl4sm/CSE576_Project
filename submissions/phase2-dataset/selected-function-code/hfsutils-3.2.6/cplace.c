void cplace(bcache *cache, bucket *b)
{
  bucket *p;

  for (p = cache->tail->cnext; p->count > 1; p = p->cnext)
    --p->count;

  b->cnext->cprev = b->cprev;
  b->cprev->cnext = b->cnext;

  if (cache->tail == b)
    cache->tail = b->cprev;

  b->cprev = p->cprev;
  b->cnext = p;

  p->cprev->cnext = b;
  p->cprev = b;
}
