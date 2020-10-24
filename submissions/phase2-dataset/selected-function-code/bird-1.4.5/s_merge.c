static inline void
s_merge(snode *from, snode *to)
{
  siterator *f, *g;

  if (!(f = from->readers))
    return;
  if (!(g = to->readers))
    {
      /* Fast path */
      to->readers = f;
      f->prev = (siterator *) to;
    fixup:
      while (f && f->node)
	{
	  f->node = NULL;
	  f = f->next;
	}
      return;
    }
  /* Really merging */
  while (g->next)
    g = g->next;
  g->next = f;
  f->prev = g;
  goto fixup;
}
