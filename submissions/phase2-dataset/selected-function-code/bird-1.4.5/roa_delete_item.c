roa_delete_item(struct roa_table *t, ip_addr prefix, byte pxlen, byte maxlen, u32 asn, byte src)
{
  struct roa_node *n = fib_find(&t->fib, &prefix, pxlen);

  if (!n)
    return;

  struct roa_item *it, **itp;
  for (itp = &n->items; it = *itp; itp = &it->next)
    if ((it->maxlen == maxlen) && (it->asn == asn) && src_match(it, src))
      break;

  if (!it)
    return;

  *itp = it->next;
  sl_free(roa_slab, it);

  // if ((n->items == NULL) && (n->n.x0 != ROA_INVALID))
  // t->cached_items++;
}
