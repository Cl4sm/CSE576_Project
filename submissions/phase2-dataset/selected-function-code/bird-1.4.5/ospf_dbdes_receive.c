ospf_dbdes_receive(struct ospf_packet *ps_i, struct ospf_iface *ifa,
		   struct ospf_neighbor *n)
{
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;

  unsigned int size = ntohs(ps_i->length);
  if (size < sizeof(struct ospf_dbdes_packet))
  {
    log(L_ERR "Bad OSPF DBDES packet from %I -  too short (%u B)", n->ip, size);
    return;
  }

  struct ospf_dbdes_packet *ps = (void *) ps_i;
  u32 ps_ddseq = ntohl(ps->ddseq);
  u32 ps_options = ntoh_opt(ps->options);
  u16 ps_iface_mtu = ntohs(ps->iface_mtu);
  
  OSPF_PACKET(ospf_dump_dbdes, ps, "DBDES packet received from %I via %s", n->ip, ifa->ifname);

  ospf_neigh_sm(n, INM_HELLOREC);

  switch (n->state)
  {
  case NEIGHBOR_DOWN:
  case NEIGHBOR_ATTEMPT:
  case NEIGHBOR_2WAY:
    return;
    break;
  case NEIGHBOR_INIT:
    ospf_neigh_sm(n, INM_2WAYREC);
    if (n->state != NEIGHBOR_EXSTART)
      return;
  case NEIGHBOR_EXSTART:

    if ((ifa->type != OSPF_IT_VLINK) && (ps_iface_mtu != ifa->iface->mtu)
	&& (ps_iface_mtu != 0) && (ifa->iface->mtu != 0))
      log(L_WARN "OSPF: MTU mismatch with neighbour %I on interface %s (remote %d, local %d)",
	  n->ip, ifa->ifname, ps_iface_mtu, ifa->iface->mtu);

    if ((ps->imms.bit.m && ps->imms.bit.ms && ps->imms.bit.i)
	&& (n->rid > po->router_id) && (size == sizeof(struct ospf_dbdes_packet)))
    {
      /* I'm slave! */
      n->dds = ps_ddseq;
      n->ddr = ps_ddseq;
      n->options = ps_options;
      n->myimms.bit.ms = 0;
      n->imms.byte = ps->imms.byte;
      OSPF_TRACE(D_PACKETS, "I'm slave to %I.", n->ip);
      ospf_neigh_sm(n, INM_NEGDONE);
      ospf_dbdes_send(n, 1);
      break;
    }

    if (((ps->imms.bit.i == 0) && (ps->imms.bit.ms == 0)) &&
        (n->rid < po->router_id) && (n->dds == ps_ddseq))
    {
      /* I'm master! */
      n->options = ps_options;
      n->ddr = ps_ddseq - 1;	/* It will be set corectly a few lines down */
      n->imms.byte = ps->imms.byte;
      OSPF_TRACE(D_PACKETS, "I'm master to %I.", n->ip);
      ospf_neigh_sm(n, INM_NEGDONE);
    }
    else
    {
      DBG("%s: Nothing happend to %I (imms=%u)\n", p->name, n->ip,
          ps->imms.byte);
      break;
    }
  case NEIGHBOR_EXCHANGE:
    if ((ps->imms.byte == n->imms.byte) && (ps_options == n->options) &&
	(ps_ddseq == n->ddr))
    {
      /* Duplicate packet */
      OSPF_TRACE(D_PACKETS, "Received duplicate dbdes from %I.", n->ip);
      if (n->myimms.bit.ms == 0)
      {
	/* Slave should retransmit dbdes packet */
	ospf_dbdes_send(n, 0);
      }
      return;
    }

    n->ddr = ps_ddseq;

    if (ps->imms.bit.ms != n->imms.bit.ms)	/* M/S bit differs */
    {
      OSPF_TRACE(D_PACKETS, "dbdes - sequence mismatch neighbor %I (bit MS)",
		 n->ip);
      ospf_neigh_sm(n, INM_SEQMIS);
      break;
    }

    if (ps->imms.bit.i)		/* I bit is set */
    {
      OSPF_TRACE(D_PACKETS, "dbdes - sequence mismatch neighbor %I (bit I)",
		 n->ip);
      ospf_neigh_sm(n, INM_SEQMIS);
      break;
    }

    n->imms.byte = ps->imms.byte;

    if (ps_options != n->options)	/* Options differs */
    {
      OSPF_TRACE(D_PACKETS, "dbdes - sequence mismatch neighbor %I (options)",
		 n->ip);
      ospf_neigh_sm(n, INM_SEQMIS);
      break;
    }

    if (n->myimms.bit.ms)
    {
      if (ps_ddseq != n->dds)	/* MASTER */
      {
	OSPF_TRACE(D_PACKETS, "dbdes - sequence mismatch neighbor %I (master)",
		   n->ip);
	ospf_neigh_sm(n, INM_SEQMIS);
	break;
      }
      n->dds++;
      DBG("Incrementing dds\n");
      ospf_dbdes_reqladd(ps, n);
      if ((n->myimms.bit.m == 0) && (ps->imms.bit.m == 0))
      {
	ospf_neigh_sm(n, INM_EXDONE);
      }
      else
      {
	ospf_dbdes_send(n, 1);
      }

    }
    else
    {
      if (ps_ddseq != (n->dds + 1))	/* SLAVE */
      {
	OSPF_TRACE(D_PACKETS, "dbdes - sequence mismatch neighbor %I (slave)", n->ip);
	ospf_neigh_sm(n, INM_SEQMIS);
	break;
      }
      n->ddr = ps_ddseq;
      n->dds = ps_ddseq;
      ospf_dbdes_reqladd(ps, n);
      ospf_dbdes_send(n, 1);
    }

    break;
  case NEIGHBOR_LOADING:
  case NEIGHBOR_FULL:
    if ((ps->imms.byte == n->imms.byte) && (ps_options == n->options)
	&& (ps_ddseq == n->ddr))
      /* Only duplicate are accepted */
    {
      OSPF_TRACE(D_PACKETS, "Received duplicate dbdes from %I.", n->ip);
      if (n->myimms.bit.ms == 0)
      {
	/* Slave should retransmit dbdes packet */
	ospf_dbdes_send(n, 0);
      }
      return;
    }
    else
    {
      OSPF_TRACE(D_PACKETS, "dbdes - sequence mismatch neighbor %I (full)",
		 n->ip);
      DBG("PS=%u, DDR=%u, DDS=%u\n", ps_ddseq, n->ddr, n->dds);
      ospf_neigh_sm(n, INM_SEQMIS);
    }
    break;
  default:
    bug("Received dbdes from %I in undefined state.", n->ip);
  }
}
