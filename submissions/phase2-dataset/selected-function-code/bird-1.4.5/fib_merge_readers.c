fib_merge_readers(struct fib_iterator *i, struct fib_node *to)
{
  if (to)
    {
      struct fib_iterator *j = to->readers;
      if (!j)
	{
	  /* Fast path */
	  to->readers = i;
	  i->prev = (struct fib_iterator *) to;
	}
      else
	{
	  /* Really merging */
	  while (j->next)
	    j = j->next;
	  j->next = i;
	  i->prev = j;
	}
      while (i && i->node)
	{
	  i->node = NULL;
	  i = i->next;
	}
    }
  else					/* No more nodes */
    while (i)
      {
	i->prev = NULL;
	i = i->next;
      }
}
