fit_init(struct fib_iterator *i, struct fib *f)
{
  unsigned h;
  struct fib_node *n;

  i->efef = 0xff;
  for(h=0; h<f->hash_size; h++)
    if (n = f->hash_table[h])
      {
	i->prev = (struct fib_iterator *) n;
	if (i->next = n->readers)
	  i->next->prev = i;
	n->readers = i;
	i->node = n;
	return;
      }
  /* The fib is empty, nothing to do */
  i->prev = i->next = NULL;
  i->node = NULL;
}
