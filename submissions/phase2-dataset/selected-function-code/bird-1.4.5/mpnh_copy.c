static struct mpnh *
mpnh_copy(struct mpnh *o)
{
  struct mpnh *first = NULL;
  struct mpnh **last = &first;

  for (; o; o = o->next)
    {
      struct mpnh *n = sl_alloc(mpnh_slab);
      n->gw = o->gw;
      n->iface = o->iface;
      n->next = NULL;
      n->weight = o->weight;

      *last = n;
      last = &(n->next);
    }

  return first;
}
