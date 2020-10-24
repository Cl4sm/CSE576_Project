check_sum_net_lsa(struct proto_ospf *po, ort *nf)
{
  struct area_net *anet = NULL;
  struct ospf_area *anet_oa = NULL;

  /* RT entry marked as area network */
  if (nf->fn.x0)
  {
    /* It is a default route for stub areas, handled entirely in ospf_rt_abr() */
    if (nf->fn.pxlen == 0)
      return;

    /* Find that area network */
    WALK_LIST(anet_oa, po->area_list)
    {
      anet = (struct area_net *) fib_find(&anet_oa->net_fib, &nf->fn.prefix, nf->fn.pxlen);
      if (anet)
	break;
    }
  }

  struct ospf_area *oa;
  WALK_LIST(oa, po->area_list)
  {
    if (anet && decide_anet_lsa(oa, anet, anet_oa))
      originate_sum_net_lsa(oa, &nf->fn, anet->metric);
    else if (decide_sum_lsa(oa, nf, ORT_NET))
      originate_sum_net_lsa(oa, &nf->fn, nf->n.metric1);
    else
      flush_sum_lsa(oa, &nf->fn, ORT_NET);
  }
}
