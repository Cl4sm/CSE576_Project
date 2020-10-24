static int
lsa_compare_for_state(const void *p1, const void *p2)
{
  struct top_hash_entry * he1 = * (struct top_hash_entry **) p1;
  struct top_hash_entry * he2 = * (struct top_hash_entry **) p2;
  struct ospf_lsa_header *lsa1 = &(he1->lsa);
  struct ospf_lsa_header *lsa2 = &(he2->lsa);

  if (he1->domain < he2->domain)
    return -1;
  if (he1->domain > he2->domain)
    return 1;

#ifdef OSPFv3
  struct ospf_lsa_header lsatmp1, lsatmp2;

  int px1 = (lsa1->type == LSA_T_PREFIX);
  int px2 = (lsa2->type == LSA_T_PREFIX);

  if (px1)
    lsa1 = fake_lsa_from_prefix_lsa(&lsatmp1, lsa1, he1->lsa_body);

  if (px2)
    lsa2 = fake_lsa_from_prefix_lsa(&lsatmp2, lsa2, he2->lsa_body);
#endif

  int nt1 = (lsa1->type == LSA_T_NET);
  int nt2 = (lsa2->type == LSA_T_NET);

  if (nt1 != nt2)
    return nt1 - nt2;

  if (nt1)
  {
#ifdef OSPFv3
    /* In OSPFv3, neworks are named base on ID of DR */
    if (lsa1->rt < lsa2->rt)
      return -1;
    if (lsa1->rt > lsa2->rt)
      return 1;
#endif

    /* For OSPFv2, this is IP of the network,
       for OSPFv3, this is interface ID */
    if (lsa1->id < lsa2->id)
      return -1;
    if (lsa1->id > lsa2->id)
      return 1;

#ifdef OSPFv3
    if (px1 != px2)
      return px1 - px2;
#endif

    return lsa1->sn - lsa2->sn;
  }
  else 
  {
    if (lsa1->rt < lsa2->rt)
      return -1;
    if (lsa1->rt > lsa2->rt)
      return 1;

    if (lsa1->type < lsa2->type)
      return -1;
    if (lsa1->type > lsa2->type)
      return 1;

    if (lsa1->id < lsa2->id)
      return -1;
    if (lsa1->id > lsa2->id)
      return 1;

#ifdef OSPFv3
    if (px1 != px2)
      return px1 - px2;
#endif
  
    return lsa1->sn - lsa2->sn;
  }
}
