rt_next_hop_update(rtable *tab)
{
  struct fib_iterator *fit = &tab->nhu_fit;
  int max_feed = 32;

  if (tab->nhu_state == 0)
    return;

  if (tab->nhu_state == 1)
    {
      FIB_ITERATE_INIT(fit, &tab->fib);
      tab->nhu_state = 2;
    }

  FIB_ITERATE_START(&tab->fib, fit, fn)
    {
      if (max_feed <= 0)
	{
	  FIB_ITERATE_PUT(fit, fn);
	  ev_schedule(tab->rt_event);
	  return;
	}
      max_feed -= rt_next_hop_update_net(tab, (net *) fn);
    }
  FIB_ITERATE_END(fn);

  /* state change 2->0, 3->1 */
  tab->nhu_state &= 1;

  if (tab->nhu_state > 0)
    ev_schedule(tab->rt_event);
}
