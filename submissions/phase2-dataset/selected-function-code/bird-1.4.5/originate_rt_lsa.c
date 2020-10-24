void
originate_rt_lsa(struct ospf_area *oa)
{
  struct ospf_lsa_header lsa;
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  void *body;

  OSPF_TRACE(D_EVENTS, "Originating router-LSA for area %R", oa->areaid);

  lsa.age = 0;
  lsa.type = LSA_T_RT;
  
#ifdef OSPFv2
  lsa.options = oa->options;
  lsa.id = po->router_id;
#else /* OSPFv3 */
  lsa.id = 0;
#endif

  lsa.rt = po->router_id;
  lsa.sn = get_seqnum(oa->rt);
  u32 dom = oa->areaid;

  body = originate_rt_lsa_body(oa, &lsa.length);
  lsasum_calculate(&lsa, body);
  oa->rt = lsa_install_new(po, &lsa, dom, body);
  ospf_lsupd_flood(po, NULL, NULL, &lsa, dom, 1);
}
