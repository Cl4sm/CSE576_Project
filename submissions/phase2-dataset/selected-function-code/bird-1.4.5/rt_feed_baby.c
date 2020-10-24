rt_feed_baby(struct proto *p)
{
  struct announce_hook *h;
  struct fib_iterator *fit;
  int max_feed = 256;

  if (!p->feed_ahook)			/* Need to initialize first */
    {
      if (!p->ahooks)
	return 1;
      DBG("Announcing routes to new protocol %s\n", p->name);
      p->feed_ahook = p->ahooks;
      fit = p->feed_iterator = mb_alloc(p->pool, sizeof(struct fib_iterator));
      goto next_hook;
    }
  fit = p->feed_iterator;

again:
  h = p->feed_ahook;
  FIB_ITERATE_START(&h->table->fib, fit, fn)
    {
      net *n = (net *) fn;
      rte *e = n->routes;
      if (max_feed <= 0)
	{
	  FIB_ITERATE_PUT(fit, fn);
	  return 0;
	}

      /* XXXX perhaps we should change feed for RA_ACCEPTED to not use 'new' */

      if ((p->accept_ra_types == RA_OPTIMAL) ||
	  (p->accept_ra_types == RA_ACCEPTED))
	if (rte_is_valid(e))
	  {
	    if (p->export_state != ES_FEEDING)
	      return 1;  /* In the meantime, the protocol fell down. */
	    do_feed_baby(p, p->accept_ra_types, h, n, e);
	    max_feed--;
	  }

      if (p->accept_ra_types == RA_ANY)
	for(e = n->routes; rte_is_valid(e); e = e->next)
	  {
	    if (p->export_state != ES_FEEDING)
	      return 1;  /* In the meantime, the protocol fell down. */
	    do_feed_baby(p, RA_ANY, h, n, e);
	    max_feed--;
	  }
    }
  FIB_ITERATE_END(fn);
  p->feed_ahook = h->next;
  if (!p->feed_ahook)
    {
      mb_free(p->feed_iterator);
      p->feed_iterator = NULL;
      return 1;
    }

next_hook:
  h = p->feed_ahook;
  FIB_ITERATE_INIT(fit, &h->table->fib);
  goto again;
}
