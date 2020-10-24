static_install(struct proto *p, struct static_route *r, struct iface *ifa)
{
  net *n;
  rta a, *aa;
  rte *e;

  if (r->installed > 0)
    return;

  DBG("Installing static route %I/%d, rtd=%d\n", r->net, r->masklen, r->dest);
  bzero(&a, sizeof(a));
  a.src = p->main_source;
  a.source = (r->dest == RTD_DEVICE) ? RTS_STATIC_DEVICE : RTS_STATIC;
  a.scope = SCOPE_UNIVERSE;
  a.cast = RTC_UNICAST;
  a.dest = r->dest;
  a.gw = r->via;
  a.iface = ifa;

  if (r->dest == RTD_MULTIPATH)
    {
      struct static_route *r2;
      struct mpnh *nhs = NULL;
      struct mpnh **nhp = &nhs;

      for (r2 = r->mp_next; r2; r2 = r2->mp_next)
	if (r2->installed)
	  {
	    struct mpnh *nh = alloca(sizeof(struct mpnh));
	    nh->gw = r2->via;
	    nh->iface = r2->neigh->iface;
	    nh->weight = r2->masklen; /* really */
	    nh->next = NULL;
	    *nhp = nh;
	    nhp = &(nh->next);
	  }

      /* There is at least one nexthop */
      if (!nhs->next)
	{
	  /* Fallback to unipath route for exactly one nexthop */
	  a.dest = RTD_ROUTER;
	  a.gw = nhs->gw;
	  a.iface = nhs->iface;
	}
      else
	a.nexthops = nhs;
    }

  if (r->dest == RTDX_RECURSIVE)
    rta_set_recursive_next_hop(p->table, &a, p_igp_table(p), &r->via, &r->via);

  aa = rta_lookup(&a);
  n = net_get(p->table, r->net, r->masklen);
  e = rte_get_temp(aa);
  e->net = n;
  e->pflags = 0;
  rte_update(p, n, e);
  r->installed = 1;
}
