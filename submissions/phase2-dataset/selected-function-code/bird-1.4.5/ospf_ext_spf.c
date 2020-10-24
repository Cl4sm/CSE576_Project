ospf_ext_spf(struct proto_ospf *po)
{
  ort *nf1, *nf2;
  orta nfa = {};
  struct top_hash_entry *en;
  struct proto *p = &po->proto;
  struct ospf_lsa_ext *le;
  int pxlen, ebit, rt_fwaddr_valid, rt_propagate;
  ip_addr ip, rtid, rt_fwaddr;
  u32 br_metric, rt_metric, rt_tag;
  struct ospf_area *atmp;

  OSPF_TRACE(D_EVENTS, "Starting routing table calculation for ext routes");

  WALK_SLIST(en, po->lsal)
  {
    /* 16.4. (1) */
    if ((en->lsa.type != LSA_T_EXT) && (en->lsa.type != LSA_T_NSSA))
      continue;

    if (en->lsa.age == LSA_MAXAGE)
      continue;

    /* 16.4. (2) */
    if (en->lsa.rt == po->router_id)
      continue;

    DBG("%s: Working on LSA. ID: %R, RT: %R, Type: %u\n",
	p->name, en->lsa.id, en->lsa.rt, en->lsa.type);

    le = en->lsa_body;

    rt_metric = le->metric & METRIC_MASK;
    ebit = le->metric & LSA_EXT_EBIT;

    if (rt_metric == LSINFINITY)
      continue;

#ifdef OSPFv2
    ip = ipa_and(ipa_from_u32(en->lsa.id), le->netmask);
    pxlen = ipa_mklen(le->netmask);
    rt_fwaddr = le->fwaddr;
    rt_fwaddr_valid = !ipa_equal(rt_fwaddr, IPA_NONE);
    rt_tag = le->tag;
    rt_propagate = en->lsa.options & OPT_P;
#else /* OSPFv3 */
    u8 pxopts;
    u16 rest;
    u32 *buf = le->rest;
    buf = lsa_get_ipv6_prefix(buf, &ip, &pxlen, &pxopts, &rest);

    if (pxopts & OPT_PX_NU)
      continue;

    rt_fwaddr_valid = le->metric & LSA_EXT_FBIT;
    if (rt_fwaddr_valid)
      buf = lsa_get_ipv6_addr(buf, &rt_fwaddr);
    else 
      rt_fwaddr = IPA_NONE;

    if (le->metric & LSA_EXT_TBIT)
      rt_tag = *buf++;
    else
      rt_tag = 0;

    rt_propagate = pxopts & OPT_PX_P;
#endif

    if (pxlen < 0 || pxlen > MAX_PREFIX_LENGTH)
    {
      log(L_WARN "%s: Invalid prefix in LSA (Type: %04x, Id: %R, Rt: %R)",
	  p->name, en->lsa.type, en->lsa.id, en->lsa.rt);
      continue;
    }


    /* 16.4. (3) */
    /* If there are more areas, we already precomputed preferred ASBR
       entries in ospf_rt_abr1() and stored them in the backbone
       table. For NSSA, we examine the area to which the LSA is assigned */
    if (en->lsa.type == LSA_T_EXT)
      atmp = ospf_main_area(po);
    else /* NSSA */
      atmp = ospf_find_area(po, en->domain);

    if (!atmp)
      continue;			/* Should not happen */

    rtid = ipa_from_rid(en->lsa.rt);
    nf1 = fib_find(&atmp->rtr, &rtid, MAX_PREFIX_LENGTH);

    if (!nf1 || !nf1->n.type)
      continue;			/* No AS boundary router found */

    if (!(nf1->n.options & ORTA_ASBR))
      continue;			/* It is not ASBR */

    /* 16.4. (3) NSSA - special rule for default routes */
    /* ABR should use default only if P-bit is set and summaries are active */
    if ((en->lsa.type == LSA_T_NSSA) && ipa_zero(ip) && (pxlen == 0) &&
	(po->areano > 1) && !(rt_propagate && atmp->ac->summary))
      continue;

    if (!rt_fwaddr_valid)
    {
      nf2 = nf1;
      nfa.nhs = nf1->n.nhs;
      br_metric = nf1->n.metric1;
    }
    else
    {
      nf2 = ospf_fib_route(&po->rtf, rt_fwaddr, MAX_PREFIX_LENGTH);
      if (!nf2)
	continue;

      if (en->lsa.type == LSA_T_EXT)
      {
	/* For ext routes, we accept intra-area or inter-area routes */
	if ((nf2->n.type != RTS_OSPF) && (nf2->n.type != RTS_OSPF_IA))
	  continue;
      }
      else /* NSSA */
      {
	/* For NSSA routes, we accept just intra-area in the same area */
	if ((nf2->n.type != RTS_OSPF) || (nf2->n.oa != atmp))
	  continue;
      }

      /* Next-hop is a part of a configured stubnet */
      if (!nf2->n.nhs)
	continue;

      nfa.nhs = nf2->n.nhs;
      br_metric = nf2->n.metric1;

      /* Replace device nexthops with nexthops to forwarding address from LSA */
      if (has_device_nexthops(nfa.nhs))
      {
	nfa.nhs = fix_device_nexthops(po, nfa.nhs, rt_fwaddr);
	nfa.nhs_reuse = 1;
      }
    }

    if (ebit)
    {
      nfa.type = RTS_OSPF_EXT2;
      nfa.metric1 = br_metric;
      nfa.metric2 = rt_metric;
    }
    else
    {
      nfa.type = RTS_OSPF_EXT1;
      nfa.metric1 = br_metric + rt_metric;
      nfa.metric2 = LSINFINITY;
    }

    /* Mark the LSA as reachable */
    en->color = INSPF;

    /* Whether the route is preferred in route selection according to 16.4.1 */
    nfa.options = epath_preferred(&nf2->n) ? ORTA_PREF : 0;
    if (en->lsa.type == LSA_T_NSSA)
    {
      nfa.options |= ORTA_NSSA;
      if (rt_propagate)
	nfa.options |= ORTA_PROP;
    }

    nfa.tag = rt_tag;
    nfa.rid = en->lsa.rt;
    nfa.oa = atmp; /* undefined in RFC 2328 */
    nfa.en = en; /* store LSA for later (NSSA processing) */

    ri_install_ext(po, ip, pxlen, &nfa);
  }
}
