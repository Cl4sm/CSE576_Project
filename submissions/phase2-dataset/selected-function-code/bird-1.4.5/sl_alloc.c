sl_alloc(slab *s)
{
  struct sl_head *h;
  struct sl_obj *o;

redo:
  h = HEAD(s->partial_heads);
  if (!h->n.next)
    goto no_partial;
okay:
  o = h->first_free;
  if (!o)
    goto full_partial;
  h->first_free = o->u.next;
  h->num_full++;
#ifdef POISON
  memset(o->u.data, 0xcd, s->data_size);
#endif
  return o->u.data;

full_partial:
  rem_node(&h->n);
  add_tail(&s->full_heads, &h->n);
  goto redo;

no_partial:
  h = HEAD(s->empty_heads);
  if (h->n.next)
    {
      rem_node(&h->n);
      add_head(&s->partial_heads, &h->n);
      s->num_empty_heads--;
      goto okay;
    }
  h = sl_new_head(s);
  add_head(&s->partial_heads, &h->n);
  goto okay;
}
