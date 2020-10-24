rt_prune_step(rtable *tab, int step, int *limit)
{
  static struct tbf rl_flush = TBF_DEFAULT_LOG_LIMITS;
  struct fib_iterator *fit = &tab->prune_fit;

  DBG("Pruning route table %s\n", tab->name);
#ifdef DEBUGGING
  fib_check(&tab->fib);
#endif

  if (tab->prune_state == RPS_NONE)
    return 1;

  if (tab->prune_state == RPS_SCHEDULED)
    {
      FIB_ITERATE_INIT(fit, &tab->fib);
      tab->prune_state = RPS_RUNNING;
    }

again:
  FIB_ITERATE_START(&tab->fib, fit, fn)
    {
      net *n = (net *) fn;
      rte *e;

    rescan:
      for (e=n->routes; e; e=e->next)
	if (e->sender->proto->flushing ||
	    (e->flags & REF_DISCARD) ||
	    (step && e->attrs->src->proto->flushing))
	  {
	    if (*limit <= 0)
	      {
		FIB_ITERATE_PUT(fit, fn);
		return 0;
	      }

	    if (step)
	      log_rl(&rl_flush, L_WARN "Route %I/%d from %s still in %s after flush",
		  n->n.prefix, n->n.pxlen, e->attrs->src->proto->name, tab->name);

	    rte_discard(tab, e);
	    (*limit)--;

	    goto rescan;
	  }
      if (!n->routes)		/* Orphaned FIB entry */
	{
	  FIB_ITERATE_PUT(fit, fn);
	  fib_delete(&tab->fib, fn);
	  goto again;
	}
    }
  FIB_ITERATE_END(fn);

#ifdef DEBUGGING
  fib_check(&tab->fib);
#endif

  tab->prune_state = RPS_NONE;
  return 1;
}
