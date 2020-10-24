static struct mpnh *
calc_next_hop(struct ospf_area *oa, struct top_hash_entry *en,
	      struct top_hash_entry *par, int pos)
{
  // struct proto *p = &oa->po->proto;
  struct proto_ospf *po = oa->po;
  struct mpnh *pn = par->nhs;
  struct ospf_iface *ifa;
  u32 rid = en->lsa.rt;

  /* 16.1.1. The next hop calculation */
  DBG("     Next hop calculating for id: %R rt: %R type: %u\n",
      en->lsa.id, en->lsa.rt, en->lsa.type);

  /* Usually, we inherit parent nexthops */
  if (inherit_nexthops(pn))
    return pn;

  /* 
   * There are three cases:
   * 1) en is a local network (and par is root)
   * 2) en is a ptp or ptmp neighbor (and par is root)
   * 3) en is a bcast or nbma neighbor (and par is local network)
   */

  /* The first case - local network */
  if ((en->lsa.type == LSA_T_NET) && (par == oa->rt))
  {
    ifa = rt_pos_to_ifa(oa, pos);
    if (!ifa)
      return NULL;

    return new_nexthop(po, IPA_NONE, ifa->iface, ifa->ecmp_weight);
  }

  /* The second case - ptp or ptmp neighbor */
  if ((en->lsa.type == LSA_T_RT) && (par == oa->rt))
  {
    ifa = rt_pos_to_ifa(oa, pos);
    if (!ifa)
      return NULL;

    if (ifa->type == OSPF_IT_VLINK)
      return new_nexthop(po, IPA_NONE, NULL, 0);

    struct ospf_neighbor *m = find_neigh(ifa, rid);
    if (!m || (m->state != NEIGHBOR_FULL))
      return NULL;

    return new_nexthop(po, m->ip, ifa->iface, ifa->ecmp_weight);
  }

  /* The third case - bcast or nbma neighbor */
  if ((en->lsa.type == LSA_T_RT) && (par->lsa.type == LSA_T_NET))
  {
    /* par->nhi should be defined from parent's calc_next_hop() */
    if (!pn)
      goto bad;

#ifdef OSPFv2
    /*
     * In this case, next-hop is the same as link-back, which is
     * already computed in link_back().
     */
    if (ipa_zero(en->lb))
      goto bad;

    return new_nexthop(po, en->lb, pn->iface, pn->weight);

#else /* OSPFv3 */
    /*
     * Next-hop is taken from lladdr field of Link-LSA, en->lb_id
     * is computed in link_back().
     */
    struct top_hash_entry *lhe;
    lhe = ospf_hash_find(po->gr, pn->iface->index, en->lb_id, rid, LSA_T_LINK);

    if (!lhe)
      return NULL;

    struct ospf_lsa_link *llsa = lhe->lsa_body;
      
    if (ipa_zero(llsa->lladdr))
      return NULL;

    return new_nexthop(po, llsa->lladdr, pn->iface, pn->weight);
#endif
  }

 bad:
  /* Probably bug or some race condition, we log it */
  log(L_ERR "Unexpected case in next hop calculation");
  return NULL;
}
