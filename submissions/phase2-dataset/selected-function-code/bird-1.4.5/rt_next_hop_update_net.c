rt_next_hop_update_net(rtable *tab, net *n)
{
  rte **k, *e, *new, *old_best, **new_best;
  int count = 0;
  int free_old_best = 0;

  old_best = n->routes;
  if (!old_best)
    return 0;

  for (k = &n->routes; e = *k; k = &e->next)
    if (rta_next_hop_outdated(e->attrs))
      {
	new = rt_next_hop_update_rte(tab, e);
	*k = new;

	rte_announce_i(tab, RA_ANY, n, new, e);
	rte_trace_in(D_ROUTES, new->sender->proto, new, "updated");

	/* Call a pre-comparison hook */
	/* Not really an efficient way to compute this */
	if (e->attrs->src->proto->rte_recalculate)
	  e->attrs->src->proto->rte_recalculate(tab, n, new, e, NULL);

	if (e != old_best)
	  rte_free_quick(e);
	else /* Freeing of the old best rte is postponed */
	  free_old_best = 1;

	e = new;
	count++;
      }

  if (!count)
    return 0;

  /* Find the new best route */
  new_best = NULL;
  for (k = &n->routes; e = *k; k = &e->next)
    {
      if (!new_best || rte_better(e, *new_best))
	new_best = k;
    }

  /* Relink the new best route to the first position */
  new = *new_best;
  if (new != n->routes)
    {
      *new_best = new->next;
      new->next = n->routes;
      n->routes = new;
    }

  /* Announce the new best route */
  if (new != old_best)
    {
      rte_announce_i(tab, RA_OPTIMAL, n, new, old_best);
      rte_trace_in(D_ROUTES, new->sender->proto, new, "updated [best]");
    }

   if (free_old_best)
    rte_free_quick(old_best);

  return count;
}
