snode *
s_get(siterator *i)
{
  siterator *f, *g;
  snode *n;

  if (!(n = i->node))
    {
      /*
       * No node found. We have to walk the iterator list backwards
       * to find where are we linked.
       */
      f = i;
      while (!f->null)
	f = f->prev;
      n = (snode *) f;
    }
  f = i->prev;				/* Maybe the snode itself */
  g = i->next;
  f->next = g;
  if (g)
    g->prev = f;

  i->prev = NULL;
  i->next = NULL;
  return n;
}
