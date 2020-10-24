struct fib_node *
fit_get(struct fib *f, struct fib_iterator *i)
{
  struct fib_node *n;
  struct fib_iterator *j, *k;

  if (!i->prev)
    {
      /* We are at the end */
      i->hash = ~0 - 1;
      return NULL;
    }
  if (!(n = i->node))
    {
      /* No node info available, we are a victim of merging. Try harder. */
      j = i;
      while (j->efef == 0xff)
	j = j->prev;
      n = (struct fib_node *) j;
    }
  j = i->prev;
  if (k = i->next)
    k->prev = j;
  j->next = k;
  i->hash = fib_hash(f, &n->prefix);
  return n;
}
