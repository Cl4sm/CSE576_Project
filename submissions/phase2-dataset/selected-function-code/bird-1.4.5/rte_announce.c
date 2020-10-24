rte_announce(rtable *tab, unsigned type, net *net, rte *new, rte *old, rte *before_old, ea_list *tmpa)
{
  if (!rte_is_valid(old))
    old = before_old = NULL;

  if (!rte_is_valid(new))
    new = NULL;

  if (!old && !new)
    return;

  if (type == RA_OPTIMAL)
    {
      if (new)
	new->attrs->src->proto->stats.pref_routes++;
      if (old)
	old->attrs->src->proto->stats.pref_routes--;

      if (tab->hostcache)
	rt_notify_hostcache(tab, net);
    }

  struct announce_hook *a;
  WALK_LIST(a, tab->hooks)
    {
      ASSERT(a->proto->export_state != ES_DOWN);
      if (a->proto->accept_ra_types == type)
	if (type == RA_ACCEPTED)
	  rt_notify_accepted(a, net, new, old, before_old, tmpa, 0);
	else
	  rt_notify_basic(a, net, new, old, tmpa, 0);
    }
}
