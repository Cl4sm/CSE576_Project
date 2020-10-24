static void
ospf_rt_sum(struct ospf_area *oa)
{
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct top_hash_entry *en;
  ip_addr ip = IPA_NONE;
  u32 dst_rid = 0;
  u32 metric, options;
  ort *abr;
  int pxlen = -1, type = -1;

  OSPF_TRACE(D_EVENTS, "Starting routing table calculation for inter-area (area %R)", oa->areaid);

  WALK_SLIST(en, po->lsal)
  {
    if ((en->lsa.type != LSA_T_SUM_RT) && (en->lsa.type != LSA_T_SUM_NET))
      continue;

    if (en->domain != oa->areaid)
      continue;

    /* 16.2. (1a) */
    if (en->lsa.age == LSA_MAXAGE)
      continue;

    /* 16.2. (2) */
    if (en->lsa.rt == po->router_id)
      continue;

    /* 16.2. (3) is handled later in ospf_rt_abr() by resetting such rt entry */

    if (en->lsa.type == LSA_T_SUM_NET)
    {
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
      options = 0;
      type = ORT_NET;
    }
    else /* LSA_T_SUM_RT */
    {
#ifdef OSPFv2
      struct ospf_lsa_sum *ls = en->lsa_body;
      dst_rid = en->lsa.id;
      options = 0;
#else /* OSPFv3 */
      struct ospf_lsa_sum_rt *ls = en->lsa_body;
      dst_rid = ls->drid; 
      options = ls->options & OPTIONS_MASK;
#endif
      
      /* We don't want local router in ASBR routing table */
      if (dst_rid == po->router_id)
	continue;

      metric = ls->metric & METRIC_MASK;
      options |= ORTA_ASBR;
      type = ORT_ROUTER;
    }

    /* 16.2. (1b) */
    if (metric == LSINFINITY)
      continue;

    /* 16.2. (4) */
    ip_addr abrip = ipa_from_rid(en->lsa.rt);
    abr = (ort *) fib_find(&oa->rtr, &abrip, MAX_PREFIX_LENGTH);
    if (!abr || !abr->n.type)
      continue;

    if (!(abr->n.options & ORTA_ABR))
      continue;

    /* This check is not mentioned in RFC 2328 */
    if (abr->n.type != RTS_OSPF)
      continue;

    /* 16.2. (5) */
    orta nf = {
      .type = RTS_OSPF_IA,
      .options = options,
      .metric1 = abr->n.metric1 + metric,
      .metric2 = LSINFINITY,
      .tag = 0,
      .rid = en->lsa.rt, /* ABR ID */
      .oa = oa,
      .nhs = abr->n.nhs
    };

    if (type == ORT_NET)
      ri_install_net(po, ip, pxlen, &nf);
    else
      ri_install_rt(oa, dst_rid, &nf);
  }
}
