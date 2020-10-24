static void
ospf_top_rehash(struct top_graph *f, int step)
{
  unsigned int oldn, oldh;
  struct top_hash_entry **n, **oldt, **newt, *e, *x;

  oldn = f->hash_size;
  oldt = f->hash_table;
  DBG("re-hashing topology hash from order %d to %d\n", f->hash_order,
      f->hash_order + step);
  f->hash_order += step;
  ospf_top_ht_alloc(f);
  newt = f->hash_table;

  for (oldh = 0; oldh < oldn; oldh++)
  {
    e = oldt[oldh];
    while (e)
    {
      x = e->next;
      n = newt + ospf_top_hash(f, e->domain, e->lsa.id, e->lsa.rt, e->lsa.type);
      e->next = *n;
      *n = e;
      e = x;
    }
  }
  ospf_top_ht_free(oldt);
}
