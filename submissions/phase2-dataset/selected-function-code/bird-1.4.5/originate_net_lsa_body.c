originate_net_lsa_body(struct ospf_iface *ifa, u16 *length,
		       struct proto_ospf *po)
{
  u16 i = 1;
  struct ospf_neighbor *n;
  struct ospf_lsa_net *net;
  int nodes = ifa->fadj + 1;

  net = mb_alloc(po->proto.pool, sizeof(struct ospf_lsa_net)
		 + nodes * sizeof(u32));

#ifdef OSPFv2
  net->netmask = ipa_mkmask(ifa->addr->pxlen);
#endif

#ifdef OSPFv3
  /* In OSPFv3, we would like to merge options from Link LSAs of added neighbors */
  struct top_hash_entry *en;
  u32 options = 0;
#endif

  net->routers[0] = po->router_id;

  WALK_LIST(n, ifa->neigh_list)
  {
    if (n->state == NEIGHBOR_FULL)
    {
#ifdef OSPFv3
      en = ospf_hash_find(po->gr, ifa->iface_id, n->iface_id, n->rid, LSA_T_LINK);
      if (en)
	options |= ((struct ospf_lsa_link *) en->lsa_body)->options;
#endif

      net->routers[i] = n->rid;
      i++;
    }
  }
  ASSERT(i == nodes);

#ifdef OSPFv3
  net->options = options & OPTIONS_MASK;
#endif
  
  *length = sizeof(struct ospf_lsa_header) + sizeof(struct ospf_lsa_net)
    + nodes * sizeof(u32);
  return net;
}
