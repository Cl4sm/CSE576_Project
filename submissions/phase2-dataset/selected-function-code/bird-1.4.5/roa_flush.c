roa_flush(struct roa_table *t, byte src)
{
  struct roa_item *it, **itp;
  struct roa_node *n;

  FIB_WALK(&t->fib, fn)
    {
      n = (struct roa_node *) fn;

      itp = &n->items;
      while (it = *itp)
	if (src_match(it, src))
	  {
	    *itp = it->next;
	    sl_free(roa_slab, it);
	  }
	else
	  itp = &it->next;
    }
  FIB_WALK_END;

  // TODO add cleanup of roa_nodes
}
