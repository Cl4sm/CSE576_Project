originate_sum_rt_lsa(struct ospf_area *oa, struct fib_node *fn, int metric, u32 options UNUSED)
{
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct top_hash_entry *en;
  u32 dom = oa->areaid;
  u32 rid = ipa_to_rid(fn->prefix);
  struct ospf_lsa_header lsa;
  void *body;

  OSPF_TRACE(D_EVENTS, "Originating rt-summary-LSA for %R (metric %d)",
	     rid, metric);

  lsa.age = 0;
#ifdef OSPFv2
  lsa.options = oa->options;
#endif
  lsa.type = LSA_T_SUM_RT;
  /* In OSPFv3, LSA ID is meaningless, but we still use Router ID of ASBR */
  lsa.id = rid;
  lsa.rt = po->router_id;

  options &= OPTIONS_MASK;
  if ((en = ospf_hash_find_header(po->gr, dom, &lsa)) != NULL)
  {
    if (check_sum_rt_lsa_same(en, lsa.id, metric, options))
      return;
  }
  lsa.sn = get_seqnum(en);

  body = originate_sum_rt_lsa_body(po, &lsa.length, lsa.id, metric, options);
  lsasum_calculate(&lsa, body);
  lsa_install_new(po, &lsa, dom, body);
  ospf_lsupd_flood(po, NULL, NULL, &lsa, dom, 1);
}
