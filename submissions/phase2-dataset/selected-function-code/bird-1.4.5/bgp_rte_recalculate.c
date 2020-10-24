int
bgp_rte_recalculate(rtable *table, net *net, rte *new, rte *old, rte *old_best)
{
  rte *r, *s;
  rte *key = new ? new : old;
  u32 lpref = key->pref;
  u32 lasn = bgp_get_neighbor(key);
  int old_is_group_best = 0;

  /*
   * Proper RFC 4271 path selection is a bit complicated, it cannot be
   * implemented just by rte_better(), because it is not a linear
   * ordering. But it can be splitted to two levels, where the lower
   * level chooses the best routes in each group of routes from the
   * same neighboring AS and higher level chooses the best route (with
   * a slightly different ordering) between the best-in-group routes.
   *
   * When deterministic_med is disabled, we just ignore this issue and
   * choose the best route by bgp_rte_better() alone. If enabled, the
   * lower level of the route selection is done here (for the group
   * to which the changed route belongs), all routes in group are
   * marked as suppressed, just chosen best-in-group is not.
   *
   * Global best route selection then implements higher level by
   * choosing between non-suppressed routes (as they are always
   * preferred over suppressed routes). Routes from BGP protocols
   * that do not set deterministic_med are just never suppressed. As
   * they do not participate in the lower level selection, it is OK
   * that this fn is not called for them.
   *
   * The idea is simple, the implementation is more problematic,
   * mostly because of optimizations in rte_recalculate() that 
   * avoids full recalculation in most cases.
   *
   * We can assume that at least one of new, old is non-NULL and both
   * are from the same protocol with enabled deterministic_med. We
   * group routes by both neighbor AS (lasn) and preference (lpref),
   * because bgp_rte_better() does not handle preference itself.
   */

  /* If new and old are from different groups, we just process that
     as two independent events */
  if (new && old && !same_group(old, lpref, lasn))
    {
      int i1, i2;
      i1 = bgp_rte_recalculate(table, net, NULL, old, old_best);
      i2 = bgp_rte_recalculate(table, net, new, NULL, old_best);
      return i1 || i2;
    }

  /* 
   * We could find the best-in-group and then make some shortcuts like
   * in rte_recalculate, but as we would have to walk through all
   * net->routes just to find it, it is probably not worth. So we
   * just have two simpler fast cases that use just the old route.
   * We also set suppressed flag to avoid using it in bgp_rte_better().
   */

  if (new)
    new->u.bgp.suppressed = 1;

  if (old)
    {
      old_is_group_best = !old->u.bgp.suppressed;
      old->u.bgp.suppressed = 1;
      int new_is_better = new && bgp_rte_better(new, old);

      /* The first case - replace not best with worse (or remove not best) */
      if (!old_is_group_best && !new_is_better)
	return 0;

      /* The second case - replace the best with better */
      if (old_is_group_best && new_is_better)
	{
	  /* new is best-in-group, the see discussion below - this is
	     a special variant of NBG && OBG. From OBG we can deduce
	     that same_group(old_best) iff (old == old_best)  */
	  new->u.bgp.suppressed = 0;
	  return (old == old_best);
	}
    }

  /* The default case - find a new best-in-group route */
  r = new; /* new may not be in the list */
  for (s=net->routes; rte_is_valid(s); s=s->next)
    if (use_deterministic_med(s) && same_group(s, lpref, lasn))
      {
	s->u.bgp.suppressed = 1;
	if (!r || bgp_rte_better(s, r))
	  r = s;
      }

  /* Simple case - the last route in group disappears */
  if (!r)
    return 0;

  /* Found best-in-group */
  r->u.bgp.suppressed = 0;

  /*
   * There are generally two reasons why we have to force
   * recalculation (return 1): First, the new route may be wrongfully
   * chosen to be the best in the first case check in
   * rte_recalculate(), this may happen only if old_best is from the
   * same group. Second, another (different than new route)
   * best-in-group is chosen and that may be the proper best (although
   * rte_recalculate() without ignore that possibility).
   *
   * There are three possible cases according to whether the old route
   * was the best in group (OBG, stored in old_is_group_best) and
   * whether the new route is the best in group (NBG, tested by r == new).
   * These cases work even if old or new is NULL.
   *
   * NBG -> new is a possible candidate for the best route, so we just
   *        check for the first reason using same_group().
   *
   * !NBG && OBG -> Second reason applies, return 1
   *
   * !NBG && !OBG -> Best in group does not change, old != old_best,
   *                 rte_better(new, old_best) is false and therefore
   *                 the first reason does not apply, return 0
   */

  if (r == new)
    return old_best && same_group(old_best, lpref, lasn);
  else
    return old_is_group_best;
}
