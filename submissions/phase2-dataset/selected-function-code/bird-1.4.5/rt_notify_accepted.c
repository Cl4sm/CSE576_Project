static void
rt_notify_accepted(struct announce_hook *ah, net *net, rte *new_changed, rte *old_changed, rte *before_old,
		   ea_list *tmpa, int feed)
{
  // struct proto *p = ah->proto;
  struct proto_stats *stats = ah->stats;

  rte *new_best = NULL;
  rte *old_best = NULL;
  rte *new_free = NULL;
  rte *old_free = NULL;
  rte *r;

  /* Used to track whether we met old_changed position. If before_old is NULL
     old_changed was the first and we met it implicitly before current best route. */
  int old_meet = old_changed && !before_old;

  /* Note that before_old is either NULL or valid (not rejected) route.
     If old_changed is valid, before_old have to be too. If old changed route
     was not valid, caller must use NULL for both old_changed and before_old. */

  if (new_changed)
    stats->exp_updates_received++;
  else
    stats->exp_withdraws_received++;

  /* First, find the new_best route - first accepted by filters */
  for (r=net->routes; rte_is_valid(r); r=r->next)
    {
      if (new_best = export_filter(ah, r, &new_free, &tmpa, 0))
	break;

      /* Note if we walked around the position of old_changed route */
      if (r == before_old)
	old_meet = 1;
    }

  /* 
   * Second, handle the feed case. That means we do not care for
   * old_best. It is NULL for feed, and the new_best for refeed. 
   * For refeed, there is a hack similar to one in rt_notify_basic()
   * to ensure withdraws in case of changed filters
   */
  if (feed)
    {
      if (feed == 2)	/* refeed */
	old_best = new_best ? new_best :
	  (rte_is_valid(net->routes) ? net->routes : NULL);
      else
	old_best = NULL;

      if (!new_best && !old_best)
	return;

      goto found;
    }

  /*
   * Now, we find the old_best route. Generally, it is the same as the
   * new_best, unless new_best is the same as new_changed or
   * old_changed is accepted before new_best.
   *
   * There are four cases:
   *
   * - We would find and accept old_changed before new_best, therefore
   *   old_changed is old_best. In remaining cases we suppose this
   *   is not true.
   *
   * - We found no new_best, therefore there is also no old_best and
   *   we ignore this withdraw.
   *
   * - We found new_best different than new_changed, therefore
   *   old_best is the same as new_best and we ignore this update.
   *
   * - We found new_best the same as new_changed, therefore it cannot
   *   be old_best and we have to continue search for old_best.
   */

  /* First case */
  if (old_meet)
    if (old_best = export_filter(ah, old_changed, &old_free, NULL, 1))
      goto found;

  /* Second case */
  if (!new_best)
    return;

  /* Third case, we use r instead of new_best, because export_filter() could change it */
  if (r != new_changed)
    {
      if (new_free)
	rte_free(new_free);
      return;
    }

  /* Fourth case */
  for (r=r->next; rte_is_valid(r); r=r->next)
    {
      if (old_best = export_filter(ah, r, &old_free, NULL, 1))
	goto found;

      if (r == before_old)
	if (old_best = export_filter(ah, old_changed, &old_free, NULL, 1))
	  goto found;
    }

  /* Implicitly, old_best is NULL and new_best is non-NULL */

 found:
  do_rt_notify(ah, net, new_best, old_best, tmpa, (feed == 2));

  /* Discard temporary rte's */
  if (new_free)
    rte_free(new_free);
  if (old_free)
    rte_free(old_free);
}
