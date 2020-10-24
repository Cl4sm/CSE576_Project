do_rt_notify(struct announce_hook *ah, net *net, rte *new, rte *old, ea_list *tmpa, int refeed)
{
  struct proto *p = ah->proto;
  struct proto_stats *stats = ah->stats;


  /*
   * First, apply export limit.
   *
   * Export route limits has several problems. Because exp_routes
   * counter is reset before refeed, we don't really know whether
   * limit is breached and whether the update is new or not. Therefore
   * the number of really exported routes may exceed the limit
   * temporarily (routes exported before and new routes in refeed).
   *
   * Minor advantage is that if the limit is decreased and refeed is
   * requested, the number of exported routes really decrease.
   *
   * Second problem is that with export limits, we don't know whether
   * old was really exported (it might be blocked by limit). When a
   * withdraw is exported, we announce it even when the previous
   * update was blocked. This is not a big issue, but the same problem
   * is in updating exp_routes counter. Therefore, to be consistent in
   * increases and decreases of exp_routes, we count exported routes
   * regardless of blocking by limits.
   *
   * Similar problem is in handling updates - when a new route is
   * received and blocking is active, the route would be blocked, but
   * when an update for the route will be received later, the update
   * would be propagated (as old != NULL). Therefore, we have to block
   * also non-new updates (contrary to import blocking).
   */

  struct proto_limit *l = ah->out_limit;
  if (l && new)
    {
      if ((!old || refeed) && (stats->exp_routes >= l->limit))
	proto_notify_limit(ah, l, PLD_OUT, stats->exp_routes);

      if (l->state == PLS_BLOCKED)
	{
	  stats->exp_routes++;	/* see note above */
	  stats->exp_updates_rejected++;
	  rte_trace_out(D_FILTERS, p, new, "rejected [limit]");
	  new = NULL;

	  if (!old)
	    return;
	}
    }


  if (new)
    stats->exp_updates_accepted++;
  else
    stats->exp_withdraws_accepted++;

  /* Hack: We do not decrease exp_routes during refeed, we instead
     reset exp_routes at the start of refeed. */
  if (new)
    stats->exp_routes++;
  if (old && !refeed)
    stats->exp_routes--;

  if (p->debug & D_ROUTES)
    {
      if (new && old)
	rte_trace_out(D_ROUTES, p, new, "replaced");
      else if (new)
	rte_trace_out(D_ROUTES, p, new, "added");
      else if (old)
	rte_trace_out(D_ROUTES, p, old, "removed");
    }
  if (!new)
    p->rt_notify(p, ah->table, net, NULL, old, NULL);
  else if (tmpa)
    {
      ea_list *t = tmpa;
      while (t->next)
	t = t->next;
      t->next = new->attrs->eattrs;
      p->rt_notify(p, ah->table, net, new, old, tmpa);
      t->next = NULL;
    }
  else
    p->rt_notify(p, ah->table, net, new, old, new->attrs->eattrs);
}
