proto_reconfigure(struct proto *p, struct proto_config *oc, struct proto_config *nc, int type)
{
  /* If the protocol is DOWN, we just restart it */
  if (p->proto_state == PS_DOWN)
    return 0;

  /* If there is a too big change in core attributes, ... */
  if ((nc->protocol != oc->protocol) ||
      (nc->disabled != p->disabled) ||
      (nc->table->table != oc->table->table))
    return 0;

  p->debug = nc->debug;
  p->mrtdump = nc->mrtdump;
  proto_reconfig_type = type;

  /* Execute protocol specific reconfigure hook */
  if (! (p->proto->reconfigure && p->proto->reconfigure(p, nc)))
    return 0;

  DBG("\t%s: same\n", oc->name);
  PD(p, "Reconfigured");
  p->cf = nc;
  p->name = nc->name;
  p->preference = nc->preference;


  /* Multitable protocols handle rest in their reconfigure hooks */
  if (p->proto->multitable)
    return 1;

  /* Update filters and limits in the main announce hook
     Note that this also resets limit state */
  if (p->main_ahook)
    {  
      struct announce_hook *ah = p->main_ahook;
      ah->in_filter = nc->in_filter;
      ah->out_filter = nc->out_filter;
      ah->rx_limit = nc->rx_limit;
      ah->in_limit = nc->in_limit;
      ah->out_limit = nc->out_limit;
      ah->in_keep_filtered = nc->in_keep_filtered;
      proto_verify_limits(ah);
    }

  /* Update routes when filters changed. If the protocol in not UP,
     it has no routes and we can ignore such changes */
  if ((p->proto_state != PS_UP) || (type == RECONFIG_SOFT))
    return 1;

  int import_changed = ! filter_same(nc->in_filter, oc->in_filter);
  int export_changed = ! filter_same(nc->out_filter, oc->out_filter);

  /* We treat a change in preferences by reimporting routes */
  if (nc->preference != oc->preference)
    import_changed = 1;

  if (import_changed || export_changed)
    log(L_INFO "Reloading protocol %s", p->name);

  /* If import filter changed, call reload hook */
  if (import_changed && ! (p->reload_routes && p->reload_routes(p)))
    {
      /* Now, the protocol is reconfigured. But route reload failed
	 and we have to do regular protocol restart. */
      log(L_INFO "Restarting protocol %s", p->name);
      p->disabled = 1;
      p->down_code = PDC_CF_RESTART;
      proto_rethink_goal(p);
      p->disabled = 0;
      proto_rethink_goal(p);
      return 1;
    }

  if (export_changed)
    proto_request_feeding(p);

  return 1;
}
