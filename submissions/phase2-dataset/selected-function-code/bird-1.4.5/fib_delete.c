void
fib_delete(struct fib *f, void *E)
{
  struct fib_node *e = E;
  unsigned int h = fib_hash(f, &e->prefix);
  struct fib_node **ee = f->hash_table + h;
  struct fib_iterator *it;

  while (*ee)
    {
      if (*ee == e)
	{
	  *ee = e->next;
	  if (it = e->readers)
	    {
	      struct fib_node *l = e->next;
	      while (!l)
		{
		  h++;
		  if (h >= f->hash_size)
		    break;
		  else
		    l = f->hash_table[h];
		}
	      fib_merge_readers(it, l);
	    }
	  sl_free(f->fib_slab, e);
	  if (f->entries-- < f->entries_min)
	    fib_rehash(f, -HASH_LO_STEP);
	  return;
	}
      ee = &((*ee)->next);
    }
  bug("fib_delete() called for invalid node");
}
