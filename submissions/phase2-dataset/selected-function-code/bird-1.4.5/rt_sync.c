rt_sync(struct proto_ospf *po)
{
  struct proto *p = &po->proto;
  struct fib_iterator fit;
  struct fib *fib = &po->rtf;
  ort *nf;
  struct ospf_area *oa;

  /* This is used for forced reload of routes */
  int reload = (po->calcrt == 2);

  OSPF_TRACE(D_EVENTS, "Starting routing table synchronisation");

  DBG("Now syncing my rt table with nest's\n");
  FIB_ITERATE_INIT(&fit, fib);
again1:
  FIB_ITERATE_START(fib, &fit, nftmp)
  {
    nf = (ort *) nftmp;

    /* Sanity check of next-hop addresses, failure should not happen */
    if (nf->n.type)
    {
      struct mpnh *nh;
      for (nh = nf->n.nhs; nh; nh = nh->next)
	if (ipa_nonzero(nh->gw))
	{
	  neighbor *ng = neigh_find2(p, &nh->gw, nh->iface, 0);
	  if (!ng || (ng->scope == SCOPE_HOST))
	    { reset_ri(nf); break; }
	}
    }

    /* Remove configured stubnets */
    if (!nf->n.nhs)
      reset_ri(nf);

    if (nf->n.type) /* Add the route */
    {
      rta a0 = {
	.src = p->main_source,
	.source = nf->n.type,
	.scope = SCOPE_UNIVERSE,
	.cast = RTC_UNICAST
      };

      if (nf->n.nhs->next)
      {
	a0.dest = RTD_MULTIPATH;
	a0.nexthops = nf->n.nhs;
      }
      else if (ipa_nonzero(nf->n.nhs->gw))
      {
	a0.dest = RTD_ROUTER;
	a0.iface = nf->n.nhs->iface;
	a0.gw = nf->n.nhs->gw;
      }
      else
      {
	a0.dest = RTD_DEVICE;
	a0.iface = nf->n.nhs->iface;
      }

      if (reload || ort_changed(nf, &a0))
      {
	net *ne = net_get(p->table, nf->fn.prefix, nf->fn.pxlen);
	rta *a = rta_lookup(&a0);
	rte *e = rte_get_temp(a);

	rta_free(nf->old_rta);
	nf->old_rta = rta_clone(a);
	e->u.ospf.metric1 = nf->old_metric1 = nf->n.metric1;
	e->u.ospf.metric2 = nf->old_metric2 = nf->n.metric2;
	e->u.ospf.tag = nf->old_tag = nf->n.tag;
	e->u.ospf.router_id = nf->old_rid = nf->n.rid;
	e->pflags = 0;
	e->net = ne;
	e->pref = p->preference;

	DBG("Mod rte type %d - %I/%d via %I on iface %s, met %d\n",
	    a0.source, nf->fn.prefix, nf->fn.pxlen, a0.gw, a0.iface ? a0.iface->name : "(none)", nf->n.metric1);
	rte_update(p, ne, e);
      }
    }
    else if (nf->old_rta)
    {
      /* Remove the route */
      rta_free(nf->old_rta);
      nf->old_rta = NULL;

      net *ne = net_get(p->table, nf->fn.prefix, nf->fn.pxlen);
      rte_update(p, ne, NULL);
    }

    /* Remove unused rt entry. Entries with fn.x0 == 1 are persistent. */
    if (!nf->n.type && !nf->fn.x0 && !nf->fn.x1)
    {
      FIB_ITERATE_PUT(&fit, nftmp);
      fib_delete(fib, nftmp);
      goto again1;
    }
  }
  FIB_ITERATE_END(nftmp);


  WALK_LIST(oa, po->area_list)
  {
    /* Cleanup ASBR hash tables */
    FIB_ITERATE_INIT(&fit, &oa->rtr);
again2:
    FIB_ITERATE_START(&oa->rtr, &fit, nftmp)
    {
      nf = (ort *) nftmp;

      if (!nf->n.type)
      {
	FIB_ITERATE_PUT(&fit, nftmp);
	fib_delete(&oa->rtr, nftmp);
	goto again2;
      }
    }
    FIB_ITERATE_END(nftmp);
  }
}
