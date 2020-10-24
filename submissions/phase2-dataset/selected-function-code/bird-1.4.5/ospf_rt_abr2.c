ospf_rt_abr2(struct proto_ospf *po)
{
  struct ospf_area *oa;
  struct top_hash_entry *en;
  ort *nf, *nf2;


  /* RFC 3103 3.1 - type-7 translator election */
  struct ospf_area *bb = po->backbone;
  WALK_LIST(oa, po->area_list)
    if (oa_is_nssa(oa))
    {
      int translate = 1;

      if (oa->ac->translator)
	goto decided;

      FIB_WALK(&oa->rtr, nftmp)
      {
	nf = (ort *) nftmp;
	if (!nf->n.type || !(nf->n.options & ORTA_ABR))
	  continue;

	nf2 = fib_find(&bb->rtr, &nf->fn.prefix, MAX_PREFIX_LENGTH);
	if (!nf2 || !nf2->n.type || !(nf2->n.options & ORTA_ABR))
	  continue;

	en = ospf_hash_find_rt(po->gr, oa->areaid, nf->n.rid);
	if (!en || (en->color != INSPF))
	  continue;

	struct ospf_lsa_rt *rt = en->lsa_body;
	/* There is better candidate - Nt-bit or higher Router ID */
	if ((rt->options & OPT_RT_NT) || (po->router_id < nf->n.rid))
	{
	  translate = 0;
	  goto decided;
	}
      }
      FIB_WALK_END;

    decided:
      if (translate && (oa->translate != TRANS_ON))
      {
	if (oa->translate == TRANS_WAIT)
	  tm_stop(oa->translator_timer);

	oa->translate = TRANS_ON;
      }

      if (!translate && (oa->translate == TRANS_ON))
      {
	if (oa->translator_timer == NULL)
	  oa->translator_timer = tm_new_set(po->proto.pool, translator_timer_hook, oa, 0, 0);

	/* Schedule the end of translation */
	tm_start(oa->translator_timer, oa->ac->transint);
	oa->translate = TRANS_WAIT;
      }
    }


  /* Compute condensed external networks */
  FIB_WALK(&po->rtf, nftmp)
  {
    nf = (ort *) nftmp;
    if (rt_is_nssa(nf) && (nf->n.options & ORTA_PROP))
    {
      struct area_net *anet = (struct area_net *)
	fib_route(&nf->n.oa->enet_fib, nf->fn.prefix, nf->fn.pxlen);

      if (anet)
      {
	if (!anet->active)
	{
	  anet->active = 1;

	  /* Get a RT entry and mark it to know that it is an area network */
	  nf2 = (ort *) fib_get(&po->rtf, &anet->fn.prefix, anet->fn.pxlen);
	  nf2->fn.x0 = 1;
	}

	u32 metric = (nf->n.type == RTS_OSPF_EXT1) ?
	  nf->n.metric1 : ((nf->n.metric2 + 1) | LSA_EXT_EBIT);

	if (anet->metric < metric)
	  anet->metric = metric;
      }
    }
  }
  FIB_WALK_END;


  FIB_WALK(&po->rtf, nftmp)
  {
    nf = (ort *) nftmp;

    check_sum_net_lsa(po, nf);
    check_nssa_lsa(po, nf);
  }
  FIB_WALK_END;
}
