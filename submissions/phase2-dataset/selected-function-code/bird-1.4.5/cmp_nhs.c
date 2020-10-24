cmp_nhs(struct mpnh *s1, struct mpnh *s2)
{
  int r;

  if (!s1)
    return 1;

  if (!s2)
    return -1;

  r = ((int) s2->weight) - ((int) s1->weight);
  if (r)
    return r;

  r = ipa_compare(s1->gw, s2->gw);
  if (r)
    return r;

  return ((int) s1->iface->index) - ((int) s2->iface->index);
}
