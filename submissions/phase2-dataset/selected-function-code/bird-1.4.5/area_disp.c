void
area_disp(struct ospf_area *oa)
{
  struct proto_ospf *po = oa->po;
  struct ospf_iface *ifa;

  /* Now try to originage rt_lsa */
  if (oa->origrt)
    update_rt_lsa(oa);

  /* Now try to originate network LSA's */
  WALK_LIST(ifa, po->iface_list)
  {
#ifdef OSPFv3
    /* Link LSA should be originated before Network LSA */
    if (ifa->origlink && (ifa->oa == oa))
      update_link_lsa(ifa);
#endif

    if (ifa->orignet && (ifa->oa == oa))
      update_net_lsa(ifa);
  }
}
