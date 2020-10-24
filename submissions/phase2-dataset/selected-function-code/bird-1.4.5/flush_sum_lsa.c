flush_sum_lsa(struct ospf_area *oa, struct fib_node *fn, int type)
{
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct top_hash_entry *en;
  struct ospf_lsa_header lsa;

  lsa.rt = po->router_id;
  if (type == ORT_NET)
    {
      lsa.id = fibnode_to_lsaid(po, fn);
      lsa.type = LSA_T_SUM_NET;
    }
  else
    {
      /* In OSPFv3, LSA ID is meaningless, but we still use Router ID of ASBR */
      lsa.id = ipa_to_rid(fn->prefix);
      lsa.type = LSA_T_SUM_RT;
    }

  if ((en = ospf_hash_find_header(po->gr, oa->areaid, &lsa)) != NULL)
    {
      OSPF_TRACE(D_EVENTS, "Flushing summary-LSA (id=%R, type=%d)",
		 en->lsa.id, en->lsa.type);

      if ((type == ORT_NET) && check_sum_net_lsaid_collision(fn, en))
	{
	  log(L_ERR "%s: LSAID collision for %I/%d",
	      p->name, fn->prefix, fn->pxlen);
	  return;
	}

      struct ospf_lsa_sum *sum = en->lsa_body;
      en->lsa.age = LSA_MAXAGE;
      en->lsa.sn = LSA_MAXSEQNO;
      lsasum_calculate(&en->lsa, sum);
      ospf_lsupd_flood(po, NULL, NULL, &en->lsa, oa->areaid, 1);
      if (can_flush_lsa(po)) flush_lsa(en, po);
    }
}
