originate_net_lsa(struct ospf_iface *ifa)
{
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;
  struct ospf_lsa_header lsa;
  u32 dom = ifa->oa->areaid;
  
  void *body;

  OSPF_TRACE(D_EVENTS, "Originating network-LSA for iface %s", ifa->ifname);

  lsa.age = 0;
  lsa.type = LSA_T_NET;

#ifdef OSPFv2
  lsa.options = ifa->oa->options;
  lsa.id = ipa_to_u32(ifa->addr->ip);
#else /* OSPFv3 */
  lsa.id = ifa->iface_id;
#endif

  lsa.rt = po->router_id;
  lsa.sn = get_seqnum(ifa->net_lsa);

  body = originate_net_lsa_body(ifa, &lsa.length, po);
  lsasum_calculate(&lsa, body);
  ifa->net_lsa = lsa_install_new(po, &lsa, dom, body);
  ospf_lsupd_flood(po, NULL, NULL, &lsa, dom, 1);
}
