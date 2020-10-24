ospf_rt_sum_tr(struct ospf_area *oa)
{
  struct proto *p = &oa->po->proto;
  struct proto_ospf *po = oa->po;
  struct ospf_area *bb = po->backbone;
  ip_addr abrip;
  struct top_hash_entry *en;
  u32 dst_rid, metric;
  ort *re = NULL, *abr;


  if (!bb) return;

  WALK_SLIST(en, po->lsal)
  {
    if ((en->lsa.type != LSA_T_SUM_RT) && (en->lsa.type != LSA_T_SUM_NET))
      continue;

    if (en->domain != oa->areaid)
      continue;

    /* 16.3 (1a) */
    if (en->lsa.age == LSA_MAXAGE)
      continue;

    /* 16.3 (2) */
    if (en->lsa.rt == po->router_id)
      continue;

    if (en->lsa.type == LSA_T_SUM_NET)
    {
      ip_addr ip;
      int pxlen;
#ifdef OSPFv2
      struct ospf_lsa_sum *ls = en->lsa_body;
      ip = ipa_and(ipa_from_u32(en->lsa.id), ls->netmask);
      pxlen = ipa_mklen(ls->netmask);
#else /* OSPFv3 */
      u8 pxopts;
      u16 rest;
      struct ospf_lsa_sum_net *ls = en->lsa_body;
      lsa_get_ipv6_prefix(ls->prefix, &ip, &pxlen, &pxopts, &rest);

      if (pxopts & OPT_PX_NU)
	continue;
#endif

      if (pxlen < 0 || pxlen > MAX_PREFIX_LENGTH)
      {
	log(L_WARN "%s: Invalid prefix in LSA (Type: %04x, Id: %R, Rt: %R)",
	    p->name, en->lsa.type, en->lsa.id, en->lsa.rt);
	continue;
      }

      metric = ls->metric & METRIC_MASK;
      re = fib_find(&po->rtf, &ip, pxlen);
    }
    else // en->lsa.type == LSA_T_SUM_RT
    {
#ifdef OSPFv2
      struct ospf_lsa_sum *ls = en->lsa_body;
      dst_rid = en->lsa.id;
#else /* OSPFv3 */
      struct ospf_lsa_sum_rt *ls = en->lsa_body;
      dst_rid = ls->drid; 
#endif

      metric = ls->metric & METRIC_MASK;
      ip_addr ip = ipa_from_rid(dst_rid);
      re = fib_find(&bb->rtr, &ip, MAX_PREFIX_LENGTH);
    }

    /* 16.3 (1b) */ 
    if (metric == LSINFINITY) 
      continue; 

    /* 16.3 (3) */
    if (!re || !re->n.type)
      continue;

    if (re->n.oa->areaid != 0)
      continue;

    if ((re->n.type != RTS_OSPF) && (re->n.type != RTS_OSPF_IA))
      continue;

    /* 16.3. (4) */
    abrip = ipa_from_rid(en->lsa.rt);
    abr = fib_find(&oa->rtr, &abrip, MAX_PREFIX_LENGTH);
    if (!abr || !abr->n.type)
      continue;

    metric = abr->n.metric1 + metric; /* IAC */

    /* 16.3. (5) */
    if ((metric < re->n.metric1) || 
	((metric == re->n.metric1) && unresolved_vlink(re)))
    {
      /* We want to replace the next-hop even if the metric is equal
	 to replace a virtual next-hop through vlink with a real one.
	 Proper ECMP would merge nexthops here, but we do not do that.
	 We restrict nexthops to fit one area to simplify check
	 12.4.3 p4 in decide_sum_lsa() */

      re->n.metric1 = metric;
      re->n.voa = oa;
      re->n.nhs = abr->n.nhs;
    }
  }
}
