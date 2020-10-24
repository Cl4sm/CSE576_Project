static inline void
check_nssa_lsa(struct proto_ospf *po, ort *nf)
{
  struct fib_node *fn = &nf->fn;
  struct area_net *anet = NULL;
  struct ospf_area *oa = NULL;
  u32 rt_metric, rt_tag;
  ip_addr rt_fwaddr;

  /* Do not translate LSA if there is already the external LSA from route export */
  if (fn->x1 == EXT_EXPORT)
    return;

  /* RT entry marked as area network */
  if (fn->x0)
  {
    /* Find that area network */
    WALK_LIST(oa, po->area_list)
    {
      anet = (struct area_net *) fib_find(&oa->enet_fib, &fn->prefix, fn->pxlen);
      if (anet)
	break;
    }
  }

  /* RFC 3103 3.2 (3) - originate the aggregated address range */
  if (anet && anet->active && !anet->hidden && oa->translate)
    originate_ext_lsa(po->backbone, fn, EXT_NSSA, anet->metric, IPA_NONE, anet->tag, 0);

  /* RFC 3103 3.2 (2) - originate the same network */
  else if (decide_nssa_lsa(nf, &rt_metric, &rt_fwaddr, &rt_tag))
    originate_ext_lsa(po->backbone, fn, EXT_NSSA, rt_metric, rt_fwaddr, rt_tag, 0);

  else if (fn->x1 == EXT_NSSA)
    flush_ext_lsa(po->backbone, fn, 0);
}
