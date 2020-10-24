add_cand(list * l, struct top_hash_entry *en, struct top_hash_entry *par,
	 u32 dist, struct ospf_area *oa, int pos)
{
  struct proto_ospf *po = oa->po;
  node *prev, *n;
  int added = 0;
  struct top_hash_entry *act;

  /* 16.1. (2b) */
  if (en == NULL)
    return;
  if (en->lsa.age == LSA_MAXAGE)
    return;

#ifdef OSPFv3
  if (en->lsa.type == LSA_T_RT)
    {
      struct ospf_lsa_rt *rt = en->lsa_body;
      if (!(rt->options & OPT_V6))
	return;
    }
#endif

  /* 16.1. (2c) */
  if (en->color == INSPF)
    return;

  /* 16.1. (2d), also checks that dist < LSINFINITY */
  if (dist > en->dist)
    return;

  /* We should check whether there is a reverse link from en to par, */
  if (!link_back(oa, en, par))
    return;

  struct mpnh *nhs = calc_next_hop(oa, en, par, pos);
  if (!nhs)
  {
    log(L_WARN "Cannot find next hop for LSA (Type: %04x, Id: %R, Rt: %R)",
	en->lsa.type, en->lsa.id, en->lsa.rt);
    return;
  }

  /* We know that en->color == CANDIDATE and en->nhs is defined. */

  if ((dist == en->dist) && !nh_is_vlink(en->nhs))
  {
    /*
     * For multipath, we should merge nexthops. We merge regular nexthops only.
     * Dummy vlink nexthops are less preferred and handled as a special case.
     *
     * During merging, new nexthops (nhs) can be reused if they are not
     * inherited from the parent (i.e. they are allocated in calc_next_hop()).
     * Current nexthops (en->nhs) can be reused if they weren't inherited in
     * previous steps (that is stored in nhs_reuse, i.e. created by merging or
     * allocated in calc_next_hop()).
     *
     * Generally, a node first inherits shared nexthops from its parent and
     * later possibly gets reusable copy during merging.
     */

    /* Keep old ones */
    if (!po->ecmp || nh_is_vlink(nhs) || (nhs == en->nhs))
      return;

    /* Merge old and new */
    int new_reuse = (par->nhs != nhs);
    en->nhs = merge_nexthops(po, en->nhs, nhs, en->nhs_reuse, new_reuse);
    en->nhs_reuse = 1;
    return;
  }

  DBG("     Adding candidate: rt: %R, id: %R, type: %u\n",
      en->lsa.rt, en->lsa.id, en->lsa.type);

  if (en->color == CANDIDATE)
  {				/* We found a shorter path */
    rem_node(&en->cn);
  }
  en->nhs = nhs;
  en->dist = dist;
  en->color = CANDIDATE;
  en->nhs_reuse = (par->nhs != nhs);

  prev = NULL;

  if (EMPTY_LIST(*l))
  {
    add_head(l, &en->cn);
  }
  else
  {
    WALK_LIST(n, *l)
    {
      act = SKIP_BACK(struct top_hash_entry, cn, n);
      if ((act->dist > dist) ||
	  ((act->dist == dist) && (act->lsa.type == LSA_T_RT)))
      {
	if (prev == NULL)
	  add_head(l, &en->cn);
	else
	  insert_node(&en->cn, prev);
	added = 1;
	break;
      }
      prev = n;
    }

    if (!added)
    {
      add_tail(l, &en->cn);
    }
  }
}
