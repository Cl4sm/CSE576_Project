static int
orta_compare_ext(const struct proto_ospf *po, const orta *new, const orta *old)
{
  int r;

  if (old->type == RTS_DUMMY)
    return 1;

  /* 16.4 (6a) - prefer routes with lower type */
  r = ((int) old->type) - ((int) new->type);
  if (r) return r;

  /* 16.4 (6b) - prefer routes with lower type 2 metric */
  if (new->type == RTS_OSPF_EXT2)
  {
    r = ((int) old->metric2) - ((int) new->metric2);
    if (r) return r;
  }

  /* 16.4 (6c) - if not RFC1583, prefer routes with preferred ASBR/next_hop */
  if (!po->rfc1583)
  {
    r = orta_pref(new) - orta_pref(old);
    if (r) return r;
  }

  /* 16.4 (6d) - prefer routes with lower type 1 metric */
  r = ((int) old->metric1) - ((int) new->metric1);
  if (r) return r;


  if (po->ecmp && po->merge_external)
    return 0;

  /*
   * RFC 3101 2.5 (6e) - prioritize Type-7 LSA with P-bit, then Type-5 LSA, then
   * LSA with higher router ID. Although this should apply just to functionally
   * equivalent LSAs (i.e. ones with the same non-zero forwarding address), we
   * use it also to disambiguate otherwise equally preferred nexthops.
   */
  if (orta_prefer_lsa(new, old))
    return 1;

  return -1;
}
