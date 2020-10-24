rt_notify_basic(struct announce_hook *ah, net *net, rte *new, rte *old, ea_list *tmpa, int refeed)
{
  // struct proto *p = ah->proto;
  struct proto_stats *stats = ah->stats;

  rte *new_free = NULL;
  rte *old_free = NULL;

  if (new)
    stats->exp_updates_received++;
  else
    stats->exp_withdraws_received++;

  /*
   * This is a tricky part - we don't know whether route 'old' was
   * exported to protocol 'p' or was filtered by the export filter.
   * We try to run the export filter to know this to have a correct
   * value in 'old' argument of rte_update (and proper filter value)
   *
   * FIXME - this is broken because 'configure soft' may change
   * filters but keep routes. Refeed is expected to be called after
   * change of the filters and with old == new, therefore we do not
   * even try to run the filter on an old route, This may lead to 
   * 'spurious withdraws' but ensure that there are no 'missing
   * withdraws'.
   *
   * This is not completely safe as there is a window between
   * reconfiguration and the end of refeed - if a newly filtered
   * route disappears during this period, proper withdraw is not
   * sent (because old would be also filtered) and the route is
   * not refeeded (because it disappeared before that).
   */

  if (new)
    new = export_filter(ah, new, &new_free, &tmpa, 0);

  if (old && !refeed)
    old = export_filter(ah, old, &old_free, NULL, 1);

  /* FIXME - This is broken because of incorrect 'old' value (see above) */
  if (!new && !old)
    return;

  do_rt_notify(ah, net, new, old, tmpa, refeed);

  /* Discard temporary rte's */
  if (new_free)
    rte_free(new_free);
  if (old_free)
    rte_free(old_free);
}
