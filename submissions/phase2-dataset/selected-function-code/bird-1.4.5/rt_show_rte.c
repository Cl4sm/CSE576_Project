static void
rt_show_rte(struct cli *c, byte *ia, rte *e, struct rt_show_data *d, ea_list *tmpa)
{
  byte via[STD_ADDRESS_P_LENGTH+32], from[STD_ADDRESS_P_LENGTH+8];
  byte tm[TM_DATETIME_BUFFER_SIZE], info[256];
  rta *a = e->attrs;
  int primary = (e->net->routes == e);
  int sync_error = (e->net->n.flags & KRF_SYNC_ERROR);
  void (*get_route_info)(struct rte *, byte *buf, struct ea_list *attrs);
  struct mpnh *nh;

  rt_format_via(e, via);
  tm_format_datetime(tm, &config->tf_route, e->lastmod);
  if (ipa_nonzero(a->from) && !ipa_equal(a->from, a->gw))
    bsprintf(from, " from %I", a->from);
  else
    from[0] = 0;

  get_route_info = a->src->proto->proto->get_route_info;
  if (get_route_info || d->verbose)
    {
      /* Need to normalize the extended attributes */
      ea_list *t = tmpa;
      t = ea_append(t, a->eattrs);
      tmpa = alloca(ea_scan(t));
      ea_merge(t, tmpa);
      ea_sort(tmpa);
    }
  if (get_route_info)
    get_route_info(e, info, tmpa);
  else
    bsprintf(info, " (%d)", e->pref);
  cli_printf(c, -1007, "%-18s %s [%s %s%s]%s%s", ia, via, a->src->proto->name,
	     tm, from, primary ? (sync_error ? " !" : " *") : "", info);
  for (nh = a->nexthops; nh; nh = nh->next)
    cli_printf(c, -1007, "\tvia %I on %s weight %d", nh->gw, nh->iface->name, nh->weight + 1);
  if (d->verbose)
    rta_show(c, a, tmpa);
}
