neigh_chstate(struct ospf_neighbor *n, u8 state)
{
  u8 oldstate;

  oldstate = n->state;

  if (oldstate != state)
  {
    struct ospf_iface *ifa = n->ifa;
    struct proto_ospf *po = ifa->oa->po;
    struct proto *p = &po->proto;

    n->state = state;

    OSPF_TRACE(D_EVENTS, "Neighbor %I changes state from \"%s\" to \"%s\".",
	       n->ip, ospf_ns[oldstate], ospf_ns[state]);

    if ((state == NEIGHBOR_2WAY) && (oldstate < NEIGHBOR_2WAY))
      ospf_iface_sm(ifa, ISM_NEICH);
    if ((state < NEIGHBOR_2WAY) && (oldstate >= NEIGHBOR_2WAY))
      ospf_iface_sm(ifa, ISM_NEICH);

    if (oldstate == NEIGHBOR_FULL)	/* Decrease number of adjacencies */
    {
      ifa->fadj--;
      schedule_rt_lsa(ifa->oa);
      if (ifa->type == OSPF_IT_VLINK) schedule_rt_lsa(ifa->voa);
      schedule_net_lsa(ifa);
    }

    if (state == NEIGHBOR_FULL)	/* Increase number of adjacencies */
    {
      ifa->fadj++;
      schedule_rt_lsa(ifa->oa);
      if (ifa->type == OSPF_IT_VLINK) schedule_rt_lsa(ifa->voa);
      schedule_net_lsa(ifa);
    }
    if (state == NEIGHBOR_EXSTART)
    {
      if (n->adj == 0)		/* First time adjacency */
      {
	n->dds = random_u32();
      }
      n->dds++;
      n->myimms.byte = 0;
      n->myimms.bit.ms = 1;
      n->myimms.bit.m = 1;
      n->myimms.bit.i = 1;
    }
    if (state > NEIGHBOR_EXSTART)
      n->myimms.bit.i = 0;
  }
}
