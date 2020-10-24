ort_merge_ext(struct proto_ospf *po, ort *o, const orta *new)
{
  orta *old = &o->n;

  if (old->nhs != new->nhs)
  {
    old->nhs = merge_nexthops(po, old->nhs, new->nhs, old->nhs_reuse, new->nhs_reuse);
    old->nhs_reuse = 1;
  }

  if (old->tag != new->tag)
    old->tag = 0;

  /*
   * Even with multipath, we store only one LSA in orta.en for the purpose of
   * NSSA/ext translation. Therefore, we apply procedures from RFC 3101 2.5 (6e)
   * to all chosen LSAs for given network, not just to functionally equivalent
   * ones (i.e. ones with the same non-zero forwarding address).
   */
  if (orta_prefer_lsa(new, old))
  {
    old->options = new->options;
    old->rid = new->rid;
    old->oa = new->oa;
    old->en = new->en;
  }
}
