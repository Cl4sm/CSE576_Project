fib_rehash(struct fib *f, int step)
{
  unsigned old, new, oldn, newn, ni, nh;
  struct fib_node **n, *e, *x, **t, **m, **h;

  old = f->hash_order;
  oldn = f->hash_size;
  new = old + step;
  m = h = f->hash_table;
  DBG("Re-hashing FIB from order %d to %d\n", old, new);
  f->hash_order = new;
  fib_ht_alloc(f);
  t = n = f->hash_table;
  newn = f->hash_size;
  ni = 0;

  while (oldn--)
    {
      x = *h++;
      while (e = x)
	{
	  x = e->next;
	  nh = fib_hash(f, &e->prefix);
	  while (nh > ni)
	    {
	      *t = NULL;
	      ni++;
	      t = ++n;
	    }
	  *t = e;
	  t = &e->next;
	}
    }
  while (ni < newn)
    {
      *t = NULL;
      ni++;
      t = ++n;
    }
  fib_ht_free(m);
}
