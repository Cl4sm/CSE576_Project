ospf_hello_receive(struct ospf_packet *ps_i, struct ospf_iface *ifa,
		   struct ospf_neighbor *n, ip_addr faddr)
{
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;
  char *beg = "OSPF: Bad HELLO packet from ";
  unsigned int size, i, twoway, peers;
  u32 tmp;
  u32 *pnrid;

  size = ntohs(ps_i->length);
  if (size < sizeof(struct ospf_hello_packet))
  {
    log(L_ERR "%s%I - too short (%u B)", beg, faddr, size);
    return;
  }

  struct ospf_hello_packet *ps = (void *) ps_i;

  OSPF_TRACE(D_PACKETS, "HELLO packet received from %I via %s", faddr, ifa->ifname);

#ifdef OSPFv2
  ip_addr mask = ps->netmask;
  ipa_ntoh(mask);
  if ((ifa->type != OSPF_IT_VLINK) &&
      (ifa->type != OSPF_IT_PTP) &&
      !ipa_equal(mask, ipa_mkmask(ifa->addr->pxlen)))
  {
    log(L_ERR "%s%I - netmask mismatch (%I)", beg, faddr, mask);
    return;
  }
#endif

  tmp = ntohs(ps->helloint);
  if (tmp != ifa->helloint)
  {
    log(L_ERR "%s%I - hello interval mismatch (%d)", beg, faddr, tmp);
    return;
  }

#ifdef OSPFv2
  tmp = ntohl(ps->deadint);
#else /* OSPFv3 */
  tmp = ntohs(ps->deadint);
#endif
  if (tmp != ifa->deadint)
  {
    log(L_ERR "%s%I - dead interval mismatch (%d)", beg, faddr, tmp);
    return;
  }

  /* Check whether bits E, N match */
  if ((ps->options ^ ifa->oa->options) & (OPT_E | OPT_N))
  {
    log(L_ERR "%s%I - area type mismatch (%x)", beg, faddr, ps->options);
    return;
  }

#ifdef OSPFv2
  if (n && (n->rid != ntohl(ps_i->routerid)))
  {
    OSPF_TRACE(D_EVENTS,
	"Neighbor %I has changed router id from %R to %R.",
	     n->ip, n->rid, ntohl(ps_i->routerid));
    ospf_neigh_remove(n);
    n = NULL;
  }
#endif

  if (!n)
  {
    if ((ifa->type == OSPF_IT_NBMA) || (ifa->type == OSPF_IT_PTMP))
    {
      struct nbma_node *nn = find_nbma_node(ifa, faddr);

      if (!nn && ifa->strictnbma)
      {
	log(L_WARN "Ignoring new neighbor: %I on %s", faddr, ifa->ifname);
	return;
      }

      if (nn && (ifa->type == OSPF_IT_NBMA) &&
	  (((ps->priority == 0) && nn->eligible) ||
	   ((ps->priority > 0) && !nn->eligible)))
      {
	log(L_ERR "Eligibility mismatch for neighbor: %I on %s", faddr, ifa->ifname);
	return;
      }

      if (nn)
	nn->found = 1;
    }

    OSPF_TRACE(D_EVENTS, "New neighbor found: %I on %s", faddr, ifa->ifname);

    n = ospf_neighbor_new(ifa);

    n->rid = ntohl(ps_i->routerid);
    n->ip = faddr;
    n->dr = ntohl(ps->dr);
    n->bdr = ntohl(ps->bdr);
    n->priority = ps->priority;
#ifdef OSPFv3
    n->iface_id = ntohl(ps->iface_id);
#endif

    if (n->ifa->cf->bfd)
      ospf_neigh_update_bfd(n, n->ifa->bfd);
  }
#ifdef OSPFv3	/* NOTE: this could also be relevant for OSPFv2 on PtP ifaces */
  else if (!ipa_equal(faddr, n->ip))
  {
    OSPF_TRACE(D_EVENTS, "Neighbor address changed from %I to %I", n->ip, faddr);
    n->ip = faddr;
  }
#endif

  ospf_neigh_sm(n, INM_HELLOREC);

  pnrid = (u32 *) ((struct ospf_hello_packet *) (ps + 1));

  peers = (size - sizeof(struct ospf_hello_packet))/ sizeof(u32);

  twoway = 0;
  for (i = 0; i < peers; i++)
  {
    if (ntohl(pnrid[i]) == po->router_id)
    {
      DBG("%s: Twoway received from %I\n", p->name, faddr);
      ospf_neigh_sm(n, INM_2WAYREC);
      twoway = 1;
      break;
    }
  }

  if (!twoway)
    ospf_neigh_sm(n, INM_1WAYREC);

  u32 olddr = n->dr;
  u32 oldbdr = n->bdr;
  u32 oldpriority = n->priority;
#ifdef OSPFv3
  u32 oldiface_id = n->iface_id;
#endif

  n->dr = ntohl(ps->dr);
  n->bdr = ntohl(ps->bdr);
  n->priority = ps->priority;
#ifdef OSPFv3
  n->iface_id = ntohl(ps->iface_id);
#endif


  /* Check priority change */
  if (n->state >= NEIGHBOR_2WAY)
  {
#ifdef OSPFv2
    u32 neigh = ipa_to_u32(n->ip);
#else /* OSPFv3 */
    u32 neigh = n->rid;
#endif

    if (n->priority != oldpriority)
      ospf_iface_sm(ifa, ISM_NEICH);

#ifdef OSPFv3
    if (n->iface_id != oldiface_id)
      ospf_iface_sm(ifa, ISM_NEICH);
#endif

    /* Neighbor is declaring itself ad DR and there is no BDR */
    if ((n->dr == neigh) && (n->bdr == 0)
	&& (n->state != NEIGHBOR_FULL))
      ospf_iface_sm(ifa, ISM_BACKS);

    /* Neighbor is declaring itself as BDR */
    if ((n->bdr == neigh) && (n->state != NEIGHBOR_FULL))
      ospf_iface_sm(ifa, ISM_BACKS);

    /* Neighbor is newly declaring itself as DR or BDR */
    if (((n->dr == neigh) && (n->dr != olddr))
	|| ((n->bdr == neigh) && (n->bdr != oldbdr)))
      ospf_iface_sm(ifa, ISM_NEICH);

    /* Neighbor is no more declaring itself as DR or BDR */
    if (((olddr == neigh) && (n->dr != olddr))
	|| ((oldbdr == neigh) && (n->bdr != oldbdr)))
      ospf_iface_sm(ifa, ISM_NEICH);
  }

  if (ifa->type == OSPF_IT_NBMA)
  {
    if ((ifa->priority == 0) && (n->priority > 0))
      ospf_hello_send(n->ifa, OHS_HELLO, n);
  }
  ospf_neigh_sm(n, INM_HELLOREC);
}
