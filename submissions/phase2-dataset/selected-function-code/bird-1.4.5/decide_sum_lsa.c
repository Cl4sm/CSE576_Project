static int
decide_sum_lsa(struct ospf_area *oa, ort *nf, int dest)
{
  /* 12.4.3.1. - for stub/NSSA areas, originating summary routes is configurable */
  if (!oa_is_ext(oa) && !oa->ac->summary)
    return 0;

  /* Invalid field - no route */
  if (!nf->n.type)
    return 0;

  /* 12.4.3 p2 */
  if (nf->n.type > RTS_OSPF_IA)
    return 0;

  /* 12.4.3 p3 */
  if ((nf->n.oa->areaid == oa->areaid))
    return 0;

  /* 12.4.3 p4 */
  if (nf->n.voa && (nf->n.voa->areaid == oa->areaid))
    return 0;

  /* 12.4.3 p5 */
  if (nf->n.metric1 >= LSINFINITY)
    return 0;

  /* 12.4.3 p6 - AS boundary router */
  if (dest == ORT_ROUTER)
  {
    /* We call decide_sum_lsa() on preferred ASBR entries, no need for 16.4. (3) */
    /* 12.4.3 p1 */
    return oa_is_ext(oa) && (nf->n.options & ORTA_ASBR);
  }

  /* 12.4.3 p7 - inter-area route */
  if (nf->n.type == RTS_OSPF_IA)
  {
    /* Inter-area routes are not repropagated into the backbone */
    return (oa != oa->po->backbone);
  }

  /* 12.4.3 p8 - intra-area route */

  /* Do not condense routing info when exporting from backbone to the transit area */
  if ((nf->n.oa == oa->po->backbone) && oa->trcap)
    return 1;

  struct area_net *anet = (struct area_net *)
    fib_route(&nf->n.oa->net_fib, nf->fn.prefix, nf->fn.pxlen);

  /* Condensed area network found */ 
  if (anet)
    return 0;

  return 1;
}
