orta_compare(const struct proto_ospf *po, const orta *new, const orta *old)
{
  int r;

  if (old->type == RTS_DUMMY)
    return 1;

  /* Prefer intra-area to inter-area to externals */
  r = ((int) old->type) - ((int) new->type);
  if (r) return r;

  /* Prefer lowest type 1 metric */
  r = ((int) old->metric1) - ((int) new->metric1);
  if (r) return r;


  /* Rest is BIRD-specific */

  /* Area-wide routes should not mix next-hops from different areas.
     This generally should not happen unless there is some misconfiguration. */
  if (new->oa->areaid != old->oa->areaid)
    return (new->oa->areaid > old->oa->areaid) ? 1 : -1;

  /* Prefer routes for configured stubnets (!nhs) to regular routes to dummy
     vlink nexthops. We intentionally return -1 if both are stubnets or vlinks. */
  if (!old->nhs)
    return -1;
  if (!new->nhs)
    return 1;
  if (nh_is_vlink(new->nhs))
    return -1;
  if (nh_is_vlink(old->nhs))
    return 1;


  if (po->ecmp)
    return 0;

  /* Prefer routes with higher Router ID, just to be more deterministic */
  if (new->rid > old->rid)
    return 1;
  
  return -1;
}
