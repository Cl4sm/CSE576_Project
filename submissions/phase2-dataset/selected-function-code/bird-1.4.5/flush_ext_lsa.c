void
flush_ext_lsa(struct ospf_area *oa, struct fib_node *fn, int nssa)
{
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct top_hash_entry *en;

  u32 dom = nssa ? oa->areaid : 0;
  u32 type = nssa ? LSA_T_NSSA : LSA_T_EXT;
  u32 lsaid = fibnode_to_lsaid(po, fn);

  if (en = ospf_hash_find(po->gr, dom, lsaid, po->router_id, type))
    {
      OSPF_TRACE(D_EVENTS, "Flushing %s-LSA for %I/%d",
		 nssa ? "NSSA" : "AS-external", fn->prefix, fn->pxlen);

      if (check_ext_lsa(en, fn, 0, IPA_NONE, 0) < 0)
	{
	  log(L_ERR "%s: LSAID collision for %I/%d",
	      p->name, fn->prefix, fn->pxlen);
	  return;
	}

      fn->x1 = 0;
      ospf_lsupd_flush_nlsa(po, en);
    }
}
