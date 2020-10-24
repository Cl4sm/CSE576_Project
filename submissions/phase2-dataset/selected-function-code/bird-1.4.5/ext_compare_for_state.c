static int
ext_compare_for_state(const void *p1, const void *p2)
{
  struct top_hash_entry * he1 = * (struct top_hash_entry **) p1;
  struct top_hash_entry * he2 = * (struct top_hash_entry **) p2;
  struct ospf_lsa_header *lsa1 = &(he1->lsa);
  struct ospf_lsa_header *lsa2 = &(he2->lsa);

  if (lsa1->rt < lsa2->rt)
    return -1;
  if (lsa1->rt > lsa2->rt)
    return 1;

  if (lsa1->id < lsa2->id)
    return -1;
  if (lsa1->id > lsa2->id)
    return 1;

  return lsa1->sn - lsa2->sn;
}
