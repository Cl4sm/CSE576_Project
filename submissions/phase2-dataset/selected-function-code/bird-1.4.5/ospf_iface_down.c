ospf_iface_down(struct ospf_iface *ifa)
{
  struct ospf_neighbor *n, *nx;
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;
  struct ospf_iface *iff;

  if (ifa->type != OSPF_IT_VLINK)
  {
#ifdef OSPFv2
    OSPF_TRACE(D_EVENTS, "Removing interface %s (%I/%d) from area %R",
	       ifa->ifname, ifa->addr->prefix, ifa->addr->pxlen, ifa->oa->areaid);
#else
    OSPF_TRACE(D_EVENTS, "Removing interface %s (IID %d) from area %R",
	       ifa->ifname, ifa->instance_id, ifa->oa->areaid);
#endif

    /* First of all kill all the related vlinks */
    WALK_LIST(iff, po->iface_list)
    {
      if ((iff->type == OSPF_IT_VLINK) && (iff->vifa == ifa))
	ospf_iface_sm(iff, ISM_DOWN);
    }
  }

  WALK_LIST_DELSAFE(n, nx, ifa->neigh_list)
  {
    OSPF_TRACE(D_EVENTS, "Removing neighbor %I", n->ip);
    ospf_neigh_remove(n);
  }

  if (ifa->hello_timer)
    tm_stop(ifa->hello_timer);

  if (ifa->poll_timer)
    tm_stop(ifa->poll_timer);

  if (ifa->wait_timer)
    tm_stop(ifa->wait_timer);

  if (ifa->type == OSPF_IT_VLINK)
  {
    ifa->vifa = NULL;
    ifa->addr = NULL;
    ifa->cost = 0;
    ifa->vip = IPA_NONE;
  }

  ifa->rt_pos_beg = 0;
  ifa->rt_pos_end = 0;
#ifdef OSPFv3
  ifa->px_pos_beg = 0;
  ifa->px_pos_end = 0;
#endif
}
