static inline int
bgp_set_next_hop(struct bgp_proto *p, rta *a)
{
  struct eattr *nh = ea_find(a->eattrs, EA_CODE(EAP_BGP, BA_NEXT_HOP));
  ip_addr *nexthop = (ip_addr *) nh->u.ptr->data;

#ifdef IPV6
  int second = (nh->u.ptr->length == NEXT_HOP_LENGTH) && ipa_nonzero(nexthop[1]);

  /* First address should not be link-local, but may be zero in direct mode */
  if (ipa_has_link_scope(*nexthop))
    *nexthop = IPA_NONE;
#else
  int second = 0;
#endif

  if (p->cf->gw_mode == GW_DIRECT)
    {
      neighbor *ng = NULL;

      if (ipa_nonzero(*nexthop))
	ng = neigh_find(&p->p, nexthop, 0);
      else if (second)	/* GW_DIRECT -> single_hop -> p->neigh != NULL */
	ng = neigh_find2(&p->p, nexthop + 1, p->neigh->iface, 0);

      /* Fallback */
      if (!ng)
	ng = p->neigh;

      if (ng->scope == SCOPE_HOST)
	return 0;

      a->dest = RTD_ROUTER;
      a->gw = ng->addr;
      a->iface = ng->iface;
      a->hostentry = NULL;
      a->igp_metric = 0;
    }
  else /* GW_RECURSIVE */
    {
      if (ipa_zero(*nexthop))
	  return 0;

      rta_set_recursive_next_hop(p->p.table, a, p->igp_table, nexthop, nexthop + second);
    }

  return 1;
}
