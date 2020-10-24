void
roa_add_item(struct roa_table *t, ip_addr prefix, byte pxlen, byte maxlen, u32 asn, byte src)
{
  struct roa_node *n = fib_get(&t->fib, &prefix, pxlen);

  // if ((n->items == NULL) && (n->n.x0 != ROA_INVALID))
  // t->cached_items--;

  struct roa_item *it;
  for (it = n->items; it; it = it->next)
    if ((it->maxlen == maxlen) && (it->asn == asn) && src_match(it, src))
      return;

  it = sl_alloc(roa_slab);
  it->asn = asn;
  it->maxlen = maxlen;
  it->src = src;
  it->next = n->items;
  n->items = it;
}
