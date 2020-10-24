void
sl_free(slab *s, void *oo)
{
  struct sl_obj *o = SKIP_BACK(struct sl_obj, u.data, oo);
  struct sl_head *h = o->slab;

#ifdef POISON
  memset(oo, 0xdb, s->data_size);
#endif
  o->u.next = h->first_free;
  h->first_free = o;
  if (!--h->num_full)
    {
      rem_node(&h->n);
      if (s->num_empty_heads >= MAX_EMPTY_HEADS)
	xfree(h);
      else
	{
	  add_head(&s->empty_heads, &h->n);
	  s->num_empty_heads++;
	}
    }
  else if (!o->u.next)
    {
      rem_node(&h->n);
      add_head(&s->partial_heads, &h->n);
    }
}
