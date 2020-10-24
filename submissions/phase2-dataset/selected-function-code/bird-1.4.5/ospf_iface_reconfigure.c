int
ospf_iface_reconfigure(struct ospf_iface *ifa, struct ospf_iface_patt *new)
{
  struct proto *p = &ifa->oa->po->proto;
  struct ospf_iface_patt *old = ifa->cf;
  char *ifname = ifa->ifname;

  /* Type could be changed in ospf_iface_new(),
     but if config values are same then also results are same */
  int old_type = ospf_iface_classify(old->type, ifa->addr);
  int new_type = ospf_iface_classify(new->type, ifa->addr);
  if (old_type != new_type)
    return 0;

  int new_stub = ospf_iface_stubby(new, ifa->addr);
  if (ifa->stub != new_stub)
    return 0;

  /* Change of these options would require to reset the iface socket */
  if ((new->real_bcast != old->real_bcast) ||
      (new->tx_tos != old->tx_tos) ||
      (new->tx_priority != old->tx_priority) ||
      (new->ttl_security != old->ttl_security))
    return 0;

  ifa->cf = new;
  ifa->marked = 0;


  /* HELLO TIMER */
  if (ifa->helloint != new->helloint)
  {
    OSPF_TRACE(D_EVENTS, "Changing hello interval on interface %s from %d to %d",
	       ifname, ifa->helloint, new->helloint);

    ifa->helloint = new->helloint;
    ospf_iface_change_timer(ifa->hello_timer, ifa->helloint);
  }

  /* RXMT TIMER */
  if (ifa->rxmtint != new->rxmtint)
  {
    OSPF_TRACE(D_EVENTS, "Changing retransmit interval on interface %s from %d to %d",
	       ifname, ifa->rxmtint, new->rxmtint);

    ifa->rxmtint = new->rxmtint;
  }

  /* POLL TIMER */
  if (ifa->pollint != new->pollint)
  {
    OSPF_TRACE(D_EVENTS, "Changing poll interval on interface %s from %d to %d",
	       ifname, ifa->pollint, new->pollint);

    ifa->pollint = new->pollint;
    ospf_iface_change_timer(ifa->poll_timer, ifa->pollint);
  }

  /* WAIT TIMER */
  if (ifa->waitint != new->waitint)
  {
    OSPF_TRACE(D_EVENTS, "Changing wait interval on interface %s from %d to %d",
	       ifname, ifa->waitint, new->waitint);

    ifa->waitint = new->waitint;
    if (ifa->wait_timer && ifa->wait_timer->expires)
      tm_start(ifa->wait_timer, ifa->waitint);
  }

  /* DEAD TIMER */
  if (ifa->deadint != new->deadint)
  {
    OSPF_TRACE(D_EVENTS, "Changing dead interval on interface %s from %d to %d",
	       ifname, ifa->deadint, new->deadint);
    ifa->deadint = new->deadint;
  }

  /* INFTRANS */
  if (ifa->inftransdelay != new->inftransdelay)
  {
    OSPF_TRACE(D_EVENTS, "Changing transmit delay on interface %s from %d to %d",
		     ifname, ifa->inftransdelay, new->inftransdelay);
    ifa->inftransdelay = new->inftransdelay;
  }

#ifdef OSPFv2	
  /* AUTHENTICATION */
  if (ifa->autype != new->autype)
  {
    OSPF_TRACE(D_EVENTS, "Changing authentication type on interface %s", ifname);
    ifa->autype = new->autype;
  }

  /* Update passwords */
  ifa->passwords = new->passwords;
#endif

  /* Remaining options are just for proper interfaces */
  if (ifa->type == OSPF_IT_VLINK)
    return 1;


  /* COST */
  if (ifa->cost != new->cost)
  {
    OSPF_TRACE(D_EVENTS, "Changing cost on interface %s from %d to %d",
	       ifname, ifa->cost, new->cost);

    ifa->cost = new->cost;
  }

  /* PRIORITY */
  if (ifa->priority != new->priority)
  {
    OSPF_TRACE(D_EVENTS, "Changing priority on interface %s from %d to %d",
	       ifname, ifa->priority, new->priority);
    ifa->priority = new->priority;
  }

  /* STRICT NBMA */
  if (ifa->strictnbma != new->strictnbma)
  {
    OSPF_TRACE(D_EVENTS, "Changing NBMA strictness on interface %s", ifname);
    ifa->strictnbma = new->strictnbma;
  }

  struct nbma_node *nb, *nbx;

  /* NBMA LIST - remove or update old */
  WALK_LIST_DELSAFE(nb, nbx, ifa->nbma_list)
  {
    struct nbma_node *nb2 = find_nbma_node_in(&new->nbma_list, nb->ip);
    if (nb2)
    {
      if (nb->eligible != nb2->eligible)
      {
	OSPF_TRACE(D_EVENTS, "Changing eligibility of neighbor %I on interface %s",
		   nb->ip, ifname);
	nb->eligible = nb2->eligible;
      }
    }
    else
    {
      OSPF_TRACE(D_EVENTS, "Removing NBMA neighbor %I on interface %s",
		       nb->ip, ifname);
      rem_node(NODE nb);
      mb_free(nb);
    }
  }

  /* NBMA LIST - add new */
  WALK_LIST(nb, new->nbma_list)
  {
    /* See related note in ospf_iface_new() */
#ifdef OSPFv2
    if (!ipa_in_net(nb->ip, ifa->addr->prefix, ifa->addr->pxlen))
      continue;
#else
    if (!ipa_has_link_scope(nb->ip))
      log(L_WARN "In OSPFv3, configured neighbor address (%I) should be link-local", nb->ip);
#endif

    if (! find_nbma_node(ifa, nb->ip))
    {
      OSPF_TRACE(D_EVENTS, "Adding NBMA neighbor %I on interface %s",
		 nb->ip, ifname);
      add_nbma_node(ifa, nb, !!find_neigh_by_ip(ifa, nb->ip));
    }
  }

  int update_buffers = 0;

  /* TX LENGTH */
  if (old->tx_length != new->tx_length)
  {
    OSPF_TRACE(D_EVENTS, "Changing TX length on interface %s from %d to %d",
	       ifname, old->tx_length, new->tx_length);

    /* ifa cannot be vlink */
    ifa->tx_length = ifa_tx_length(ifa);
    update_buffers = 1;
  }

  /* RX BUFFER */
  if (old->rx_buffer != new->rx_buffer)
  {
    OSPF_TRACE(D_EVENTS, "Changing buffer size on interface %s from %d to %d",
	       ifname, old->rx_buffer, new->rx_buffer);

    /* ifa cannot be vlink */
    update_buffers = 1;
  }

  /* Buffer size depends on both tx_length and rx_buffer options */
  if (update_buffers && ifa->sk)
  {
    uint bsize = ifa_bufsize(ifa);
    sk_set_rbsize(ifa->sk, bsize);
    sk_set_tbsize(ifa->sk, bsize);
  }

  /* LINK */
  if (ifa->check_link != new->check_link)
  {
    OSPF_TRACE(D_EVENTS, "%s link check on interface %s",
	       new->check_link ? "Enabling" : "Disabling", ifname);
    ifa->check_link = new->check_link;

    /* ifa cannot be vlink */
    if (!(ifa->iface->flags & IF_LINK_UP))
      ospf_iface_sm(ifa, ifa->check_link ? ISM_LOOP : ISM_UNLOOP);
  }

  /* ECMP weight */
  if (ifa->ecmp_weight != new->ecmp_weight)
  {
    OSPF_TRACE(D_EVENTS, "Changing ECMP weight of interface %s from %d to %d",
	       ifname, (int)ifa->ecmp_weight + 1, (int)new->ecmp_weight + 1);
    ifa->ecmp_weight = new->ecmp_weight;
  }

  /* BFD */
  if (ifa->bfd != new->bfd)
  {
    OSPF_TRACE(D_EVENTS, "%s BFD on interface %s",
	       new->bfd ? "Enabling" : "Disabling", ifname);
    ifa->bfd = new->bfd;

    struct ospf_neighbor *n;
    WALK_LIST(n, ifa->neigh_list)
      ospf_neigh_update_bfd(n, ifa->bfd);
  }


  /* instance_id is not updated - it is part of key */

  return 1;
}
