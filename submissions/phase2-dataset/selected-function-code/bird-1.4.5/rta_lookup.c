rta_lookup(rta *o)
{
  rta *r;
  unsigned int h;

  ASSERT(!(o->aflags & RTAF_CACHED));
  if (o->eattrs)
    {
      if (o->eattrs->next)	/* Multiple ea_list's, need to merge them */
	{
	  ea_list *ml = alloca(ea_scan(o->eattrs));
	  ea_merge(o->eattrs, ml);
	  o->eattrs = ml;
	}
      ea_sort(o->eattrs);
    }

  h = rta_hash(o);
  for(r=rta_hash_table[h & rta_cache_mask]; r; r=r->next)
    if (r->hash_key == h && rta_same(r, o))
      return rta_clone(r);

  r = rta_copy(o);
  r->hash_key = h;
  r->aflags = RTAF_CACHED;
  rt_lock_source(r->src);
  rt_lock_hostentry(r->hostentry);
  rta_insert(r);

  if (++rta_cache_count > rta_cache_limit)
    rta_rehash();

  return r;
}
