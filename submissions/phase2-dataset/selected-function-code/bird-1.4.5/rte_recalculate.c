rte_recalculate(struct announce_hook *ah, net *net, rte *new, ea_list *tmpa, struct rte_src *src)
{
  struct proto *p = ah->proto;
  struct rtable *table = ah->table;
  struct proto_stats *stats = ah->stats;
  static struct tbf rl_pipe = TBF_DEFAULT_LOG_LIMITS;
  rte *before_old = NULL;
  rte *old_best = net->routes;
  rte *old = NULL;
  rte **k;

  k = &net->routes;			/* Find and remove original route from the same protocol */
  while (old = *k)
    {
      if (old->attrs->src == src)
	{
	  /* If there is the same route in the routing table but from
	   * a different sender, then there are two paths from the
	   * source protocol to this routing table through transparent
	   * pipes, which is not allowed.
	   *
	   * We log that and ignore the route. If it is withdraw, we
	   * ignore it completely (there might be 'spurious withdraws',
	   * see FIXME in do_rte_announce())
	   */
	  if (old->sender->proto != p)
	    {
	      if (new)
		{
		  log_rl(&rl_pipe, L_ERR "Pipe collision detected when sending %I/%d to table %s",
		      net->n.prefix, net->n.pxlen, table->name);
		  rte_free_quick(new);
		}
	      return;
	    }

	  if (new && rte_same(old, new))
	    {
	      /* No changes, ignore the new route */

	      if (!rte_is_filtered(new))
		{
		  stats->imp_updates_ignored++;
		  rte_trace_in(D_ROUTES, p, new, "ignored");
		}

	      rte_free_quick(new);
#ifdef CONFIG_RIP
	      /* lastmod is used internally by RIP as the last time
		 when the route was received. */
	      if (src->proto->proto == &proto_rip)
		old->lastmod = now;
#endif
	      return;
	    }
	  *k = old->next;
	  break;
	}
      k = &old->next;
      before_old = old;
    }

  if (!old)
    before_old = NULL;

  if (!old && !new)
    {
      stats->imp_withdraws_ignored++;
      return;
    }

  int new_ok = rte_is_ok(new);
  int old_ok = rte_is_ok(old);

  struct proto_limit *l = ah->rx_limit;
  if (l && !old && new)
    {
      u32 all_routes = stats->imp_routes + stats->filt_routes;

      if (all_routes >= l->limit)
	proto_notify_limit(ah, l, PLD_RX, all_routes);

      if (l->state == PLS_BLOCKED)
	{
	  /* In receive limit the situation is simple, old is NULL so
	     we just free new and exit like nothing happened */

	  stats->imp_updates_ignored++;
	  rte_trace_in(D_FILTERS, p, new, "ignored [limit]");
	  rte_free_quick(new);
	  return;
	}
    }

  l = ah->in_limit;
  if (l && !old_ok && new_ok)
    {
      if (stats->imp_routes >= l->limit)
	proto_notify_limit(ah, l, PLD_IN, stats->imp_routes);

      if (l->state == PLS_BLOCKED)
	{
	  /* In import limit the situation is more complicated. We
	     shouldn't just drop the route, we should handle it like
	     it was filtered. We also have to continue the route
	     processing if old or new is non-NULL, but we should exit
	     if both are NULL as this case is probably assumed to be
	     already handled. */

	  stats->imp_updates_ignored++;
	  rte_trace_in(D_FILTERS, p, new, "ignored [limit]");

	  if (ah->in_keep_filtered)
	    new->flags |= REF_FILTERED;
	  else
	    { rte_free_quick(new); new = NULL; }

	  /* Note that old && !new could be possible when
	     ah->in_keep_filtered changed in the recent past. */

	  if (!old && !new)
	    return;

	  new_ok = 0;
	  goto skip_stats1;
	}
    }

  if (new_ok)
    stats->imp_updates_accepted++;
  else if (old_ok)
    stats->imp_withdraws_accepted++;
  else
    stats->imp_withdraws_ignored++;

 skip_stats1:

  if (new)
    rte_is_filtered(new) ? stats->filt_routes++ : stats->imp_routes++;
  if (old)
    rte_is_filtered(old) ? stats->filt_routes-- : stats->imp_routes--;

  if (table->config->sorted)
    {
      /* If routes are sorted, just insert new route to appropriate position */
      if (new)
	{
	  if (before_old && !rte_better(new, before_old))
	    k = &before_old->next;
	  else
	    k = &net->routes;

	  for (; *k; k=&(*k)->next)
	    if (rte_better(new, *k))
	      break;

	  new->next = *k;
	  *k = new;
	}
    }
  else
    {
      /* If routes are not sorted, find the best route and move it on
	 the first position. There are several optimized cases. */

      if (src->proto->rte_recalculate && src->proto->rte_recalculate(table, net, new, old, old_best))
	goto do_recalculate;

      if (new && rte_better(new, old_best))
	{
	  /* The first case - the new route is cleary optimal,
	     we link it at the first position */

	  new->next = net->routes;
	  net->routes = new;
	}
      else if (old == old_best)
	{
	  /* The second case - the old best route disappeared, we add the
	     new route (if we have any) to the list (we don't care about
	     position) and then we elect the new optimal route and relink
	     that route at the first position and announce it. New optimal
	     route might be NULL if there is no more routes */

	do_recalculate:
	  /* Add the new route to the list */
	  if (new)
	    {
	      new->next = net->routes;
	      net->routes = new;
	    }

	  /* Find a new optimal route (if there is any) */
	  if (net->routes)
	    {
	      rte **bp = &net->routes;
	      for (k=&(*bp)->next; *k; k=&(*k)->next)
		if (rte_better(*k, *bp))
		  bp = k;

	      /* And relink it */
	      rte *best = *bp;
	      *bp = best->next;
	      best->next = net->routes;
	      net->routes = best;
	    }
	}
      else if (new)
	{
	  /* The third case - the new route is not better than the old
	     best route (therefore old_best != NULL) and the old best
	     route was not removed (therefore old_best == net->routes).
	     We just link the new route after the old best route. */

	  ASSERT(net->routes != NULL);
	  new->next = net->routes->next;
	  net->routes->next = new;
	}
      /* The fourth (empty) case - suboptimal route was removed, nothing to do */
    }

  if (new)
    new->lastmod = now;

  /* Log the route change */
  if (p->debug & D_ROUTES)
    {
      if (new_ok)
	rte_trace(p, new, '>', new == net->routes ? "added [best]" : "added");
      else if (old_ok)
	{
	  if (old != old_best)
	    rte_trace(p, old, '>', "removed");
	  else if (rte_is_ok(net->routes))
	    rte_trace(p, old, '>', "removed [replaced]");
	  else
	    rte_trace(p, old, '>', "removed [sole]");
	}
    }

  /* Propagate the route change */
  rte_announce(table, RA_ANY, net, new, old, NULL, tmpa);
  if (net->routes != old_best)
    rte_announce(table, RA_OPTIMAL, net, net->routes, old_best, NULL, tmpa);
  if (table->config->sorted)
    rte_announce(table, RA_ACCEPTED, net, new, old, before_old, tmpa);

  if (!net->routes &&
      (table->gc_counter++ >= table->config->gc_max_ops) &&
      (table->gc_time + table->config->gc_min_time <= now))
    rt_schedule_gc(table);

  if (old_ok && p->rte_remove)
    p->rte_remove(net, old);
  if (new_ok && p->rte_insert)
    p->rte_insert(net, new);

  if (old)
    rte_free_quick(old);
}
