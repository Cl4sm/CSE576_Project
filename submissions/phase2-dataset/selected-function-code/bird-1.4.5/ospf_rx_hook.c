int
ospf_rx_hook(sock *sk, int size)
{
  char *mesg = "OSPF: Bad packet from ";

  /* We want just packets from sk->iface. Unfortunately, on BSD we
     cannot filter out other packets at kernel level and we receive
     all packets on all sockets */
  if (sk->lifindex != sk->iface->index)
    return 1;

  DBG("OSPF: RX hook called (iface %s, src %I, dst %I)\n",
      sk->ifname, sk->faddr, sk->laddr);

  /* Initially, the packet is associated with the 'master' iface */
  struct ospf_iface *ifa = sk->data;
  struct proto_ospf *po = ifa->oa->po;
  // struct proto *p = &po->proto;

  int src_local, dst_local UNUSED, dst_mcast; 
  src_local = ipa_in_net(sk->faddr, ifa->addr->prefix, ifa->addr->pxlen);
  dst_local = ipa_equal(sk->laddr, ifa->addr->ip);
  dst_mcast = ipa_equal(sk->laddr, ifa->all_routers) || ipa_equal(sk->laddr, AllDRouters);

#ifdef OSPFv2
  /* First, we eliminate packets with strange address combinations.
   * In OSPFv2, they might be for other ospf_ifaces (with different IP
   * prefix) on the same real iface, so we don't log it. We enforce
   * that (src_local || dst_local), therefore we are eliminating all
   * such cases. 
   */
  if (dst_mcast && !src_local)
    return 1;
  if (!dst_mcast && !dst_local)
    return 1;

  /* Ignore my own broadcast packets */
  if (ifa->cf->real_bcast && ipa_equal(sk->faddr, ifa->addr->ip))
    return 1;
#else /* OSPFv3 */

  /* In OSPFv3, src_local and dst_local mean link-local. 
   * RFC 5340 says that local (non-vlink) packets use
   * link-local src address, but does not enforce it. Strange.
   */
  if (dst_mcast && !src_local)
    log(L_WARN "OSPF: Received multicast packet from %I (not link-local)", sk->faddr);
#endif

  /* Second, we check packet size, checksum, and the protocol version */
  struct ospf_packet *ps = (struct ospf_packet *) ip_skip_header(sk->rbuf, &size);

  if (ps == NULL)
  {
    log(L_ERR "%s%I - bad IP header", mesg, sk->faddr);
    return 1;
  }

  if (ifa->check_ttl && (sk->rcv_ttl < 255))
  {
    log(L_ERR "%s%I - TTL %d (< 255)", mesg, sk->faddr, sk->rcv_ttl);
    return 1;
  }

  if ((unsigned) size < sizeof(struct ospf_packet))
  {
    log(L_ERR "%s%I - too short (%u bytes)", mesg, sk->faddr, size);
    return 1;
  }

  uint plen = ntohs(ps->length);
  if ((plen < sizeof(struct ospf_packet)) || ((plen % 4) != 0))
  {
    log(L_ERR "%s%I - invalid length (%u)", mesg, sk->faddr, plen);
    return 1;
  }

  if (sk->flags & SKF_TRUNCATED)
  {
    log(L_WARN "%s%I - too large (%d/%d)", mesg, sk->faddr, plen, size);

    /* If we have dynamic buffers and received truncated message, we expand RX buffer */

    uint bs = plen + 256;
    bs = BIRD_ALIGN(bs, 1024);

    if (!ifa->cf->rx_buffer && (bs > sk->rbsize))
      sk_set_rbsize(sk, bs);

    return 1;
  }

  if (plen > size)
  {
    log(L_ERR "%s%I - size field does not match (%d/%d)", mesg, sk->faddr, plen, size);
    return 1;
  }

  if (ps->version != OSPF_VERSION)
  {
    log(L_ERR "%s%I - version %u", mesg, sk->faddr, ps->version);
    return 1;
  }

#ifdef OSPFv2
  if ((ps->autype != htons(OSPF_AUTH_CRYPT)) &&
      (!ipsum_verify(ps, 16, (void *) ps + sizeof(struct ospf_packet),
		     plen - sizeof(struct ospf_packet), NULL)))
  {
    log(L_ERR "%s%I - bad checksum", mesg, sk->faddr);
    return 1;
  }
#endif


  /* Third, we resolve associated iface and handle vlinks. */

  u32 areaid = ntohl(ps->areaid);
  u32 rid = ntohl(ps->routerid);

  if ((areaid == ifa->oa->areaid)
#ifdef OSPFv3
      && (ps->instance_id == ifa->instance_id)
#endif
      )
  {
    /* It is real iface, source should be local (in OSPFv2) */
#ifdef OSPFv2
    if (!src_local)
      return 1;
#endif
  }
  else if (dst_mcast || (areaid != 0))
  {
    /* Obvious mismatch */

#ifdef OSPFv2
    /* We ignore mismatch in OSPFv3, because there might be
       other instance with different instance ID */
    log(L_ERR "%s%I - area does not match (%R vs %R)",
	mesg, sk->faddr, areaid, ifa->oa->areaid);
#endif
    return 1;
  }
  else
  {
    /* Some vlink? */
    struct ospf_iface *iff = NULL;

    WALK_LIST(iff, po->iface_list)
    {
      if ((iff->type == OSPF_IT_VLINK) && 
	  (iff->voa == ifa->oa) &&
#ifdef OSPFv3
	  (iff->instance_id == ps->instance_id) &&
#endif
	  (iff->vid == rid))
	{
	  /* Vlink should be UP */
	  if (iff->state != OSPF_IS_PTP)
	    return 1;
	  
	  ifa = iff;
	  goto found;
	}
    }

#ifdef OSPFv2
    log(L_WARN "OSPF: Received packet for unknown vlink (ID %R, IP %I)", rid, sk->faddr);
#endif
    return 1;
  }

 found:
  if (ifa->stub)	    /* This shouldn't happen */
    return 1;

  if (ipa_equal(sk->laddr, AllDRouters) && (ifa->sk_dr == 0))
    return 1;

  if (rid == po->router_id)
  {
    log(L_ERR "%s%I - received my own router ID!", mesg, sk->faddr);
    return 1;
  }

  if (rid == 0)
  {
    log(L_ERR "%s%I - router id = 0.0.0.0", mesg, sk->faddr);
    return 1;
  }

#ifdef OSPFv2
  /* In OSPFv2, neighbors are identified by either IP or Router ID, base on network type */
  struct ospf_neighbor *n;
  if ((ifa->type == OSPF_IT_BCAST) || (ifa->type == OSPF_IT_NBMA) || (ifa->type == OSPF_IT_PTMP))
    n = find_neigh_by_ip(ifa, sk->faddr);
  else
    n = find_neigh(ifa, rid);
#else
  struct ospf_neighbor *n = find_neigh(ifa, rid);
#endif

  if(!n && (ps->type != HELLO_P))
  {
    log(L_WARN "OSPF: Received non-hello packet from unknown neighbor (src %I, iface %s)",
	sk->faddr, ifa->ifname);
    return 1;
  }

  if (!ospf_pkt_checkauth(n, ifa, ps, size))
  {
    log(L_ERR "%s%I - authentication failed", mesg, sk->faddr);
    return 1;
  }

  /* Dump packet 
     pu8=(u8 *)(sk->rbuf+5*4);
     for(i=0;i<ntohs(ps->length);i+=4)
     DBG("%s: received %u,%u,%u,%u\n",p->name, pu8[i+0], pu8[i+1], pu8[i+2],
     pu8[i+3]);
     DBG("%s: received size: %u\n",p->name,size);
   */

  switch (ps->type)
  {
  case HELLO_P:
    DBG("%s: Hello received.\n", p->name);
    ospf_hello_receive(ps, ifa, n, sk->faddr);
    break;
  case DBDES_P:
    DBG("%s: Database description received.\n", p->name);
    ospf_dbdes_receive(ps, ifa, n);
    break;
  case LSREQ_P:
    DBG("%s: Link state request received.\n", p->name);
    ospf_lsreq_receive(ps, ifa, n);
    break;
  case LSUPD_P:
    DBG("%s: Link state update received.\n", p->name);
    ospf_lsupd_receive(ps, ifa, n);
    break;
  case LSACK_P:
    DBG("%s: Link state ack received.\n", p->name);
    ospf_lsack_receive(ps, ifa, n);
    break;
  default:
    log(L_ERR "%s%I - wrong type %u", mesg, sk->faddr, ps->type);
    return 1;
  };
  return 1;
}
