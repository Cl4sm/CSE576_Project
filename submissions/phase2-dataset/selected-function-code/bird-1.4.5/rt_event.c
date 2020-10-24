rt_event(void *ptr)
{
  rtable *tab = ptr;

  if (tab->hcu_scheduled)
    rt_update_hostcache(tab);

  if (tab->nhu_state)
    rt_next_hop_update(tab);

  if (tab->prune_state)
    if (!rt_prune_table(tab))
      {
	/* Table prune unfinished */
	ev_schedule(tab->rt_event);
	return;
      }

  if (tab->gc_scheduled)
    {
      rt_prune_nets(tab);
      rt_prune_sources(); // FIXME this should be moved to independent event
    }
}
