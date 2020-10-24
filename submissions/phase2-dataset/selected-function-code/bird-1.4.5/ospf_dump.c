static void
ospf_dump(struct proto *p)
{
  struct ospf_iface *ifa;
  struct ospf_neighbor *n;
  struct proto_ospf *po = (struct proto_ospf *) p;

  OSPF_TRACE(D_EVENTS, "Area number: %d", po->areano);

  WALK_LIST(ifa, po->iface_list)
  {
    OSPF_TRACE(D_EVENTS, "Interface: %s", ifa->ifname);
    OSPF_TRACE(D_EVENTS, "state: %u", ifa->state);
    OSPF_TRACE(D_EVENTS, "DR:  %R", ifa->drid);
    OSPF_TRACE(D_EVENTS, "BDR: %R", ifa->bdrid);
    WALK_LIST(n, ifa->neigh_list)
    {
      OSPF_TRACE(D_EVENTS, "  neighbor %R in state %u", n->rid, n->state);
    }
  }

  /*
  OSPF_TRACE(D_EVENTS, "LSA graph dump start:");
  ospf_top_dump(po->gr, p);
  OSPF_TRACE(D_EVENTS, "LSA graph dump finished");
  */
  neigh_dump_all();
}
