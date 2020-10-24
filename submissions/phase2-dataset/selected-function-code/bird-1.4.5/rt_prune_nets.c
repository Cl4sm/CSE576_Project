rt_prune_nets(rtable *tab)
{
  struct fib_iterator fit;
  int ncnt = 0, ndel = 0;

#ifdef DEBUGGING
  fib_check(&tab->fib);
#endif

  FIB_ITERATE_INIT(&fit, &tab->fib);
again:
  FIB_ITERATE_START(&tab->fib, &fit, f)
    {
      net *n = (net *) f;
      ncnt++;
      if (!n->routes)		/* Orphaned FIB entry */
	{
	  FIB_ITERATE_PUT(&fit, f);
	  fib_delete(&tab->fib, f);
	  ndel++;
	  goto again;
	}
    }
  FIB_ITERATE_END(f);
  DBG("Pruned %d of %d networks\n", ndel, ncnt);

  tab->gc_counter = 0;
  tab->gc_time = now;
  tab->gc_scheduled = 0;
}
