bgp_rehash_buckets(struct bgp_proto *p)
{
  struct bgp_bucket **old = p->bucket_hash;
  struct bgp_bucket **new;
  unsigned oldn = p->hash_size;
  unsigned i, e, mask;
  struct bgp_bucket *b;

  p->hash_size = p->hash_limit;
  DBG("BGP: Rehashing bucket table from %d to %d\n", oldn, p->hash_size);
  p->hash_limit *= 4;
  if (p->hash_limit >= 65536)
    p->hash_limit = ~0;
  new = p->bucket_hash = mb_allocz(p->p.pool, p->hash_size * sizeof(struct bgp_bucket *));
  mask = p->hash_size - 1;
  for (i=0; i<oldn; i++)
    while (b = old[i])
      {
	old[i] = b->hash_next;
	e = b->hash & mask;
	b->hash_next = new[e];
	if (b->hash_next)
	  b->hash_next->hash_prev = b;
	b->hash_prev = NULL;
	new[e] = b;
      }
  mb_free(old);
}
