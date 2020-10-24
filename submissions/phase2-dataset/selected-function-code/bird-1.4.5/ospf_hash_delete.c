void
ospf_hash_delete(struct top_graph *f, struct top_hash_entry *e)
{
  struct top_hash_entry **ee = f->hash_table + 
    ospf_top_hash(f, e->domain, e->lsa.id, e->lsa.rt, e->lsa.type);

  while (*ee)
  {
    if (*ee == e)
    {
      *ee = e->next;
      sl_free(f->hash_slab, e);
      if (f->hash_entries-- < f->hash_entries_min)
	ospf_top_rehash(f, -HASH_LO_STEP);
      return;
    }
    ee = &((*ee)->next);
  }
  bug("ospf_hash_delete() called for invalid node");
}
