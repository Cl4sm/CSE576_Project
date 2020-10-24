static void *
originate_rt_lsa_body(struct ospf_area *oa, u16 *length)
{
  struct proto_ospf *po = oa->po;
  struct ospf_iface *ifa;
  int i = 0, bitv = 0;
  struct ospf_lsa_rt *rt;
  struct ospf_lsa_rt_link *ln;
  struct ospf_neighbor *neigh;

  ASSERT(po->lsab_used == 0);
  rt = lsab_allocz(po, sizeof(struct ospf_lsa_rt));

  rt->options = 0;

  if (po->areano > 1)
    rt->options |= OPT_RT_B;

  if ((po->areano > 1) && oa_is_nssa(oa) && oa->ac->translator)
    rt->options |= OPT_RT_NT;

  if (po->ebit && !oa_is_stub(oa))
    rt->options |= OPT_RT_E;

  rt = NULL; /* buffer might be reallocated later */

  WALK_LIST(ifa, po->iface_list)
  {
    int net_lsa = 0;
    u32 link_cost = po->stub_router ? 0xffff : ifa->cost;

    if ((ifa->type == OSPF_IT_VLINK) && (ifa->voa == oa) &&
	(!EMPTY_LIST(ifa->neigh_list)))
    {
      neigh = (struct ospf_neighbor *) HEAD(ifa->neigh_list);
      if ((neigh->state == NEIGHBOR_FULL) && (ifa->cost <= 0xffff))
	bitv = 1;
    }

    if ((ifa->oa != oa) || (ifa->state == OSPF_IS_DOWN))
      continue;

    ifa->rt_pos_beg = i;

    /* RFC2328 - 12.4.1.1-4 */
    switch (ifa->type)
      {
      case OSPF_IT_PTP:
      case OSPF_IT_PTMP:
	WALK_LIST(neigh, ifa->neigh_list)
	  if (neigh->state == NEIGHBOR_FULL)
	  {
	    ln = lsab_alloc(po, sizeof(struct ospf_lsa_rt_link));
	    ln->type = LSART_PTP;
	    ln->id = neigh->rid;

	    /*
	     * ln->data should be ifa->iface_id in case of no/ptp
	     * address (ifa->addr->flags & IA_PEER) on PTP link (see
	     * RFC 2328 12.4.1.1.), but the iface ID value has no use,
	     * while using IP address even in this case is here for
	     * compatibility with some broken implementations that use
	     * this address as a next-hop.
	     */
	    ln->data = ipa_to_u32(ifa->addr->ip);
	    ln->metric = link_cost;
	    ln->padding = 0;
	    i++;
	  }
	break;

      case OSPF_IT_BCAST:
      case OSPF_IT_NBMA:
	if (bcast_net_active(ifa))
	  {
	    ln = lsab_alloc(po, sizeof(struct ospf_lsa_rt_link));
	    ln->type = LSART_NET;
	    ln->id = ipa_to_u32(ifa->drip);
	    ln->data = ipa_to_u32(ifa->addr->ip);
	    ln->metric = link_cost;
	    ln->padding = 0;
	    i++;
	    net_lsa = 1;
	  }
	break;

      case OSPF_IT_VLINK:
	neigh = (struct ospf_neighbor *) HEAD(ifa->neigh_list);
	if ((!EMPTY_LIST(ifa->neigh_list)) && (neigh->state == NEIGHBOR_FULL) && (ifa->cost <= 0xffff))
	{
	  ln = lsab_alloc(po, sizeof(struct ospf_lsa_rt_link));
	  ln->type = LSART_VLNK;
	  ln->id = neigh->rid;
	  ln->data = ipa_to_u32(ifa->addr->ip);
	  ln->metric = link_cost;
	  ln->padding = 0;
	  i++;
        }
        break;

      default:
        log("Unknown interface type %s", ifa->ifname);
        break;
      }

    ifa->rt_pos_end = i;

    /* Now we will originate stub area if there is no primary */
    if (net_lsa ||
	(ifa->type == OSPF_IT_VLINK) ||
	((ifa->addr->flags & IA_PEER) && ! ifa->cf->stub) ||
	configured_stubnet(oa, ifa->addr))
      continue;

    ln = lsab_alloc(po, sizeof(struct ospf_lsa_rt_link));
    if ((ifa->addr->flags & IA_HOST) ||
	(ifa->state == OSPF_IS_LOOP) ||
	(ifa->type == OSPF_IT_PTMP))
    {
      /* Host stub entry */
      ln->type = LSART_STUB;
      ln->id = ipa_to_u32(ifa->addr->ip);
      ln->data = 0xffffffff;
      ln->metric = 0;
      ln->padding = 0;
    }
    else 
    {
      /* Network stub entry */
      ln->type = LSART_STUB;
      ln->id = ipa_to_u32(ifa->addr->prefix);
      ln->data = ipa_to_u32(ipa_mkmask(ifa->addr->pxlen));
      ln->metric = ifa->cost;
      ln->padding = 0;
    }
    i++;

    ifa->rt_pos_end = i;
  }

  struct ospf_stubnet_config *sn;
  if (oa->ac)
    WALK_LIST(sn, oa->ac->stubnet_list)
      if (!sn->hidden)
      {
	ln = lsab_alloc(po, sizeof(struct ospf_lsa_rt_link));
	ln->type = LSART_STUB;
	ln->id = ipa_to_u32(sn->px.addr);
	ln->data = ipa_to_u32(ipa_mkmask(sn->px.len));
	ln->metric = sn->cost;
	ln->padding = 0;
	i++;
      }

  rt = po->lsab;
  rt->links = i;

  if (bitv) 
    rt->options |= OPT_RT_V;

  *length = po->lsab_used + sizeof(struct ospf_lsa_header);
  return lsab_flush(po);
}
