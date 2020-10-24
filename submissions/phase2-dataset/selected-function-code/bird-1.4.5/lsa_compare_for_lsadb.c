static int
lsa_compare_for_lsadb(const void *p1, const void *p2)
{
  struct top_hash_entry * he1 = * (struct top_hash_entry **) p1;
  struct top_hash_entry * he2 = * (struct top_hash_entry **) p2;
  struct ospf_lsa_header *lsa1 = &(he1->lsa);
  struct ospf_lsa_header *lsa2 = &(he2->lsa);
  int sc1 = LSA_SCOPE(lsa1);
  int sc2 = LSA_SCOPE(lsa2);

  if (sc1 != sc2)
    return sc2 - sc1;

  if (he1->domain != he2->domain)
    return he1->domain - he2->domain;

  if (lsa1->rt != lsa2->rt)
    return lsa1->rt - lsa2->rt;
  
  if (lsa1->id != lsa2->id)
    return lsa1->id - lsa2->id;

  if (lsa1->type != lsa2->type)
    return lsa1->type - lsa2->type;

  return lsa1->sn - lsa2->sn;
}
