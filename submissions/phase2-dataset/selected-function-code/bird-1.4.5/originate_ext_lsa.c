void
originate_ext_lsa(struct ospf_area *oa, struct fib_node *fn, int src,
		  u32 metric, ip_addr fwaddr, u32 tag, int pbit)
{
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct ospf_lsa_header lsa;
  struct top_hash_entry *en = NULL;
  void *body;
  int nssa = oa_is_nssa(oa);
  u32 dom = nssa ? oa->areaid : 0;

  OSPF_TRACE(D_EVENTS, "Originating %s-LSA for %I/%d",
	     nssa ? "NSSA" : "AS-external", fn->prefix, fn->pxlen);

  lsa.age = 0;
#ifdef OSPFv2
  lsa.options = nssa ? (pbit ? OPT_P : 0) : OPT_E;
#endif
  lsa.type = nssa ? LSA_T_NSSA : LSA_T_EXT;
  lsa.id = fibnode_to_lsaid(po, fn);
  lsa.rt = po->router_id;

  if (nssa && pbit && ipa_zero(fwaddr))
  {
    /* NSSA-LSA with P-bit set must have non-zero forwarding address */

    fwaddr = find_surrogate_fwaddr(oa);
    if (ipa_zero(fwaddr))
    {
      log(L_ERR "%s: Cannot find forwarding address for NSSA-LSA %I/%d",
	  p->name, fn->prefix, fn->pxlen);
      return;
    }
  }

  if ((en = ospf_hash_find_header(po->gr, dom, &lsa)) != NULL)
  {
    int rv = check_ext_lsa(en, fn, metric, fwaddr, tag);
    if (rv < 0)
    {
      log(L_ERR "%s: LSAID collision for %I/%d",
	  p->name, fn->prefix, fn->pxlen);
      return;
    }

    if (rv > 0)
      return;
  }
  lsa.sn = get_seqnum(en);

  body = originate_ext_lsa_body(po, &lsa.length, fn, metric, fwaddr, tag, pbit);
  lsasum_calculate(&lsa, body);

  if (src) 
    fn->x1 = src;

  lsa_install_new(po, &lsa, dom, body);
  ospf_lsupd_flood(po, NULL, NULL, &lsa, dom, 1);

  if (po->ebit == 0)
  {
    po->ebit = 1;
    WALK_LIST(oa, po->area_list)
    {
      schedule_rt_lsa(oa);
    }
  }
}
