void
ospf_iface_new(struct ospf_area *oa, struct ifa *addr, struct ospf_iface_patt *ip)
{
  struct proto *p = &oa->po->proto;
  struct iface *iface = addr->iface;
  struct ospf_iface *ifa;
  struct pool *pool;

#ifdef OSPFv2
  OSPF_TRACE(D_EVENTS, "Adding interface %s (%I/%d) to area %R",
	     iface->name, addr->prefix, addr->pxlen, oa->areaid);
#else
  OSPF_TRACE(D_EVENTS, "Adding interface %s (IID %d) to area %R",
	     iface->name, ip->instance_id, oa->areaid);
#endif

  pool = rp_new(p->pool, "OSPF Interface");
  ifa = mb_allocz(pool, sizeof(struct ospf_iface));
  ifa->iface = iface;
  ifa->addr = addr;
  ifa->oa = oa;
  ifa->cf = ip;
  ifa->pool = pool;

  ifa->iface_id = iface->index;
  ifa->ifname = iface->name;

  ifa->cost = ip->cost;
  ifa->rxmtint = ip->rxmtint;
  ifa->inftransdelay = ip->inftransdelay;
  ifa->priority = ip->priority;
  ifa->helloint = ip->helloint;
  ifa->pollint = ip->pollint;
  ifa->strictnbma = ip->strictnbma;
  ifa->waitint = ip->waitint;
  ifa->deadint = ip->deadint;
  ifa->stub = ospf_iface_stubby(ip, addr);
  ifa->ioprob = OSPF_I_OK;

  ifa->tx_length = ifa_tx_length(ifa);
  ifa->check_link = ip->check_link;
  ifa->ecmp_weight = ip->ecmp_weight;
  ifa->check_ttl = (ip->ttl_security == 1);
  ifa->bfd = ip->bfd;

#ifdef OSPFv2
  ifa->autype = ip->autype;
  ifa->passwords = ip->passwords;
  ifa->ptp_netmask = !(addr->flags & IA_PEER);
  if (ip->ptp_netmask < 2)
    ifa->ptp_netmask = ip->ptp_netmask;
#endif

#ifdef OSPFv3
  ifa->instance_id = ip->instance_id;
#endif


  ifa->type = ospf_iface_classify(ip->type, addr);

  /* Check validity of interface type */
  int old_type = ifa->type;
  u32 if_multi_flag = ip->real_bcast ? IF_BROADCAST : IF_MULTICAST;

#ifdef OSPFv2
  if ((ifa->type == OSPF_IT_BCAST) && (addr->flags & IA_PEER))
    ifa->type = OSPF_IT_PTP;

  if ((ifa->type == OSPF_IT_NBMA) && (addr->flags & IA_PEER))
    ifa->type = OSPF_IT_PTMP;
#endif

  if ((ifa->type == OSPF_IT_BCAST) && !(iface->flags & if_multi_flag))
    ifa->type = OSPF_IT_NBMA;

  if ((ifa->type == OSPF_IT_PTP) && !(iface->flags & if_multi_flag))
    ifa->type = OSPF_IT_PTMP;

  if (ifa->type != old_type)
    log(L_WARN "%s: Cannot use interface %s as %s, forcing %s",
	p->name, iface->name, ospf_it[old_type], ospf_it[ifa->type]);


  ifa->state = OSPF_IS_DOWN;
  init_list(&ifa->neigh_list);
  init_list(&ifa->nbma_list);

  struct nbma_node *nb;
  WALK_LIST(nb, ip->nbma_list)
  {
    /* In OSPFv3, addr is link-local while configured neighbors could
       have global IP (although RFC 5340 C.5 says link-local addresses
       should be used). Because OSPFv3 iface is not subnet-specific,
       there is no need for ipa_in_net() check */

#ifdef OSPFv2
    if (!ipa_in_net(nb->ip, addr->prefix, addr->pxlen))
      continue;
#else
    if (!ipa_has_link_scope(nb->ip))
      log(L_WARN "In OSPFv3, configured neighbor address (%I) should be link-local", nb->ip);
#endif

    add_nbma_node(ifa, nb, 0);
  }

  add_tail(&oa->po->iface_list, NODE ifa);

  /*
   * In some cases we allow more ospf_ifaces on one physical iface.
   * In OSPFv2, if they use different IP address prefix.
   * In OSPFv3, if they use different instance_id.
   * Therefore, we store such info to lock->addr field.
   */

  struct object_lock *lock = olock_new(pool);
#ifdef OSPFv2
  lock->addr = ifa->addr->prefix;
#else /* OSPFv3 */
  lock->addr = _MI(0,0,0,ifa->instance_id);
#endif
  lock->type = OBJLOCK_IP;
  lock->port = OSPF_PROTO;
  lock->iface = iface;
  lock->data = ifa;
  lock->hook = ospf_iface_add;

  olock_acquire(lock);
}
