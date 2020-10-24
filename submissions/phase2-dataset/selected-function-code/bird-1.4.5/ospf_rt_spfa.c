static void
ospf_rt_spfa(struct ospf_area *oa)
{
  struct proto *p = &oa->po->proto;
  struct proto_ospf *po = oa->po;
  struct ospf_lsa_rt *rt;
  struct ospf_lsa_net *ln;
  struct top_hash_entry *act, *tmp;
  ip_addr prefix UNUSED;
  int pxlen UNUSED;
  u32 i, *rts;
  node *n;

  if (oa->rt == NULL)
    return;

  OSPF_TRACE(D_EVENTS, "Starting routing table calculation for area %R", oa->areaid);

  /* 16.1. (1) */
  init_list(&oa->cand);		/* Empty list of candidates */
  oa->trcap = 0;

  DBG("LSA db prepared, adding me into candidate list.\n");

  oa->rt->dist = 0;
  oa->rt->color = CANDIDATE;
  add_head(&oa->cand, &oa->rt->cn);
  DBG("RT LSA: rt: %R, id: %R, type: %u\n",
      oa->rt->lsa.rt, oa->rt->lsa.id, oa->rt->lsa.type);

  while (!EMPTY_LIST(oa->cand))
  {
    n = HEAD(oa->cand);
    act = SKIP_BACK(struct top_hash_entry, cn, n);
    rem_node(n);

    DBG("Working on LSA: rt: %R, id: %R, type: %u\n",
	act->lsa.rt, act->lsa.id, act->lsa.type);

    act->color = INSPF;
    switch (act->lsa.type)
    {
    case LSA_T_RT:
      rt = (struct ospf_lsa_rt *) act->lsa_body;
      if (rt->options & OPT_RT_V)
	oa->trcap = 1;

      /*
       * In OSPFv3, all routers are added to per-area routing
       * tables. But we use it just for ASBRs and ABRs. For the
       * purpose of the last step in SPF - prefix-LSA processing in
       * process_prefixes(), we use information stored in LSA db.
       */
      if (((rt->options & OPT_RT_E) || (rt->options & OPT_RT_B))
	  && (act->lsa.rt != po->router_id))
      {
	orta nf = {
	  .type = RTS_OSPF,
	  .options = rt->options,
	  .metric1 = act->dist,
	  .metric2 = LSINFINITY,
	  .tag = 0,
	  .rid = act->lsa.rt,
	  .oa = oa,
	  .nhs = act->nhs
	};
	ri_install_rt(oa, act->lsa.rt, &nf);
      }

#ifdef OSPFv2
      ospf_rt_spfa_rtlinks(oa, act, act);
#else /* OSPFv3 */
      /* Errata 2078 to RFC 5340 4.8.1 - skip links from non-routing nodes */
      if ((act != oa->rt) && !(rt->options & OPT_R))
	break;

      for (tmp = ospf_hash_find_rt_first(po->gr, act->domain, act->lsa.rt);
	   tmp; tmp = ospf_hash_find_rt_next(tmp))
	ospf_rt_spfa_rtlinks(oa, act, tmp);
#endif

      break;
    case LSA_T_NET:
      ln = act->lsa_body;

#ifdef OSPFv2
      prefix = ipa_and(ipa_from_u32(act->lsa.id), ln->netmask);
      pxlen = ipa_mklen(ln->netmask);
      add_network(oa, prefix, pxlen, act->dist, act, -1);
#endif

      rts = (u32 *) (ln + 1);
      for (i = 0; i < lsa_net_count(&act->lsa); i++)
      {
	DBG("     Working on router %R ", rts[i]);
	tmp = ospf_hash_find_rt(po->gr, oa->areaid, rts[i]);
	if (tmp != NULL)
	  DBG("Found :-)\n");
	else
	  DBG("Not found!\n");
	add_cand(&oa->cand, tmp, act, act->dist, oa, -1);
      }
      break;
    }
  }

#ifdef OSPFv3
  process_prefixes(oa);
#endif
}
