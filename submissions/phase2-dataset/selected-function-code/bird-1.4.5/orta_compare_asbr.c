orta_compare_asbr(const struct proto_ospf *po, const orta *new, const orta *old)
{
  int r;

  if (old->type == RTS_DUMMY)
    return 1;

  if (!po->rfc1583)
  {
    r = epath_preferred(new) - epath_preferred(old);
    if (r) return r;
  }

  r = ((int) old->metric1) - ((int) new->metric1);
  if (r) return r;

  /* Larger area ID is preferred */
  if (new->oa->areaid > old->oa->areaid)
    return 1;

  /* There is just one ASBR of that RID per area, so tie is not possible */
  return -1;
}
