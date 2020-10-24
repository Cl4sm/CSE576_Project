originate_sum_net_lsa(struct ospf_area *oa, struct fib_node *fn, int metric)
{
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct top_hash_entry *en;
  u32 dom = oa->areaid;
  struct ospf_lsa_header lsa;
  void *body;

  OSPF_TRACE(D_EVENTS, "Originating net-summary-LSA for %I/%d (metric %d)",
	     fn->prefix, fn->pxlen, metric);

  /* options argument is used in ORT_NET and OSPFv3 only */
  lsa.age = 0;
#ifdef OSPFv2
  lsa.options = oa->options;
#endif
  lsa.type = LSA_T_SUM_NET;
  lsa.id = fibnode_to_lsaid(po, fn);
  lsa.rt = po->router_id;

  if ((en = ospf_hash_find_header(po->gr, dom, &lsa)) != NULL)
  {
    if (check_sum_net_lsaid_collision(fn, en))
    {
      log(L_ERR "%s: LSAID collision for %I/%d",
	  p->name, fn->prefix, fn->pxlen);
      return;
    }

    if (check_sum_net_lsa_same(en, metric))
      return;
  }
  lsa.sn = get_seqnum(en);

  body = originate_sum_net_lsa_body(po, &lsa.length, fn, metric);
  lsasum_calculate(&lsa, body);
  lsa_install_new(po, &lsa, dom, body);
  ospf_lsupd_flood(po, NULL, NULL, &lsa, dom, 1);
}
