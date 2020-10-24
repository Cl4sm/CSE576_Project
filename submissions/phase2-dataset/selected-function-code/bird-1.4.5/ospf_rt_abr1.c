ospf_rt_abr1(struct proto_ospf *po)
{
  struct area_net *anet;
  ort *nf, *default_nf;

  /* RFC 2328 G.3 - incomplete resolution of virtual next hops - routers */
  FIB_WALK(&po->backbone->rtr, nftmp)
  {
    nf = (ort *) nftmp;

    if (nf->n.type && unresolved_vlink(nf))
      reset_ri(nf);
  }
  FIB_WALK_END;


  FIB_WALK(&po->rtf, nftmp)
  {
    nf = (ort *) nftmp;


    /* RFC 2328 G.3 - incomplete resolution of virtual next hops - networks */
    if (nf->n.type && unresolved_vlink(nf))
      reset_ri(nf);


    /* Compute condensed area networks */
    if (nf->n.type == RTS_OSPF)
    {
      anet = (struct area_net *) fib_route(&nf->n.oa->net_fib, nf->fn.prefix, nf->fn.pxlen);
      if (anet)
      {
	if (!anet->active)
	{
	  anet->active = 1;

	  /* Get a RT entry and mark it to know that it is an area network */
	  ort *nfi = (ort *) fib_get(&po->rtf, &anet->fn.prefix, anet->fn.pxlen);
	  nfi->fn.x0 = 1; /* mark and keep persistent, to have stable UID */

	  /* 16.2. (3) */
	  if (nfi->n.type == RTS_OSPF_IA)
	    reset_ri(nfi);
	}

	if (anet->metric < nf->n.metric1)
	  anet->metric = nf->n.metric1;
      }
    }
  }
  FIB_WALK_END;

  ip_addr addr = IPA_NONE;
  default_nf = (ort *) fib_get(&po->rtf, &addr, 0);
  default_nf->fn.x0 = 1; /* keep persistent */

  struct ospf_area *oa;
  WALK_LIST(oa, po->area_list)
  {

    /* 12.4.3.1. - originate or flush default route for stub/NSSA areas */
    if (oa_is_stub(oa) || (oa_is_nssa(oa) && !oa->ac->summary))
      originate_sum_net_lsa(oa, &default_nf->fn, oa->ac->default_cost);
    else
      flush_sum_lsa(oa, &default_nf->fn, ORT_NET);

    /*
     * Originate type-7 default route for NSSA areas
     *
     * Because type-7 default LSAs are originated by ABRs, they do not
     * collide with other type-7 LSAs (as ABRs generate type-5 LSAs
     * for both external route export or external-NSSA translation),
     * so we use 0 for the src arg.
     */

    if (oa_is_nssa(oa) && oa->ac->default_nssa)
      originate_ext_lsa(oa, &default_nf->fn, 0, oa->ac->default_cost, IPA_NONE, 0, 0);
    else
      flush_ext_lsa(oa, &default_nf->fn, 1);


    /* RFC 2328 16.4. (3) - precompute preferred ASBR entries */
    if (oa_is_ext(oa))
    {
      FIB_WALK(&oa->rtr, nftmp)
      {
	nf = (ort *) nftmp;
	if (nf->n.options & ORTA_ASBR)
	  ri_install_asbr(po, &nf->fn.prefix, &nf->n);
      }
      FIB_WALK_END;
    }
  }


  /* Originate or flush ASBR summary LSAs */
  FIB_WALK(&po->backbone->rtr, nftmp)
  {
    check_sum_rt_lsa(po, (ort *) nftmp);
  }
  FIB_WALK_END;


  /* RFC 2328 16.7. p2 - find new/lost vlink endpoints */
  ospf_check_vlinks(po);
}
