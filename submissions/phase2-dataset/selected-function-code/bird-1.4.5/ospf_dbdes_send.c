void
ospf_dbdes_send(struct ospf_neighbor *n, int next)
{
  struct ospf_dbdes_packet *pkt;
  struct ospf_packet *op;
  struct ospf_iface *ifa = n->ifa;
  struct ospf_area *oa = ifa->oa;
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  u16 length, i, j;

  /* FIXME ??? */
  if ((oa->rt == NULL) || (EMPTY_LIST(po->lsal)))
    update_rt_lsa(oa);

  switch (n->state)
  {
  case NEIGHBOR_EXSTART:	/* Send empty packets */
    n->myimms.bit.i = 1;
    pkt = ospf_tx_buffer(ifa);
    op = &pkt->ospf_packet;
    ospf_pkt_fill_hdr(ifa, pkt, DBDES_P);
    pkt->iface_mtu = (ifa->type == OSPF_IT_VLINK) ? 0 : htons(ifa->iface->mtu);
    pkt->options = hton_opt(oa->options);
    pkt->imms = n->myimms;
    pkt->ddseq = htonl(n->dds);
    length = sizeof(struct ospf_dbdes_packet);
    op->length = htons(length);

    OSPF_PACKET(ospf_dump_dbdes, pkt, "DBDES packet sent to %I via %s", n->ip, ifa->ifname);
    ospf_send_to(ifa, n->ip);
    break;

  case NEIGHBOR_EXCHANGE:
    n->myimms.bit.i = 0;

    if (next)
    {
      snode *sn;
      struct ospf_lsa_header *lsa;

      if (n->ldd_bsize != ifa->tx_length)
      {
	mb_free(n->ldd_buffer);
	n->ldd_buffer = mb_allocz(n->pool, ifa->tx_length);
	n->ldd_bsize = ifa->tx_length;
      }

      pkt = n->ldd_buffer;
      op = (struct ospf_packet *) pkt;

      ospf_pkt_fill_hdr(ifa, pkt, DBDES_P);
      pkt->iface_mtu = (ifa->type == OSPF_IT_VLINK) ? 0 : htons(ifa->iface->mtu);
      pkt->ddseq = htonl(n->dds);
      pkt->options = hton_opt(oa->options);

      j = i = (ospf_pkt_maxsize(ifa) - sizeof(struct ospf_dbdes_packet)) / sizeof(struct ospf_lsa_header);	/* Number of possible lsaheaders to send */
      lsa = (n->ldd_buffer + sizeof(struct ospf_dbdes_packet));

      if (n->myimms.bit.m)
      {
	sn = s_get(&(n->dbsi));

	DBG("Number of LSA: %d\n", j);
	for (; i > 0; i--)
	{
	  struct top_hash_entry *en= (struct top_hash_entry *) sn;

          if (ospf_lsa_flooding_allowed(&en->lsa, en->domain, ifa))
          {
	    htonlsah(&(en->lsa), lsa);
	    DBG("Working on: %d\n", i);
	    DBG("\tX%01x %-1R %-1R %p\n", en->lsa.type, en->lsa.id, en->lsa.rt, en->lsa_body);

	    lsa++;
          }
          else i++;	/* No lsa added */

	  if (sn == STAIL(po->lsal))
          {
            i--;
	    break;
          }

	  sn = sn->next;
	}

	if (sn == STAIL(po->lsal))
	{
	  DBG("Number of LSA NOT sent: %d\n", i);
	  DBG("M bit unset.\n");
	  n->myimms.bit.m = 0;	/* Unset more bit */
	}

	s_put(&(n->dbsi), sn);
      }

      pkt->imms.byte = n->myimms.byte;

      length = (j - i) * sizeof(struct ospf_lsa_header) +
	sizeof(struct ospf_dbdes_packet);
      op->length = htons(length);

      DBG("%s: DB_DES (M) prepared for %I.\n", p->name, n->ip);
    }

  case NEIGHBOR_LOADING:
  case NEIGHBOR_FULL:
    length = n->ldd_buffer ? ntohs(((struct ospf_packet *) n->ldd_buffer)->length) : 0;

    if (!length)
    {
      OSPF_TRACE(D_PACKETS, "No packet in my buffer for repeating");
      ospf_neigh_sm(n, INM_KILLNBR);
      return;
    }

    /* Send last packet from ldd buffer */

    OSPF_PACKET(ospf_dump_dbdes, n->ldd_buffer, "DBDES packet sent to %I via %s", n->ip, ifa->ifname);

    sk_set_tbuf(ifa->sk, n->ldd_buffer);
    ospf_send_to(ifa, n->ip);
    sk_set_tbuf(ifa->sk, NULL);

    if(n->myimms.bit.ms) tm_start(n->rxmt_timer, n->ifa->rxmtint);		/* Restart timer */

    if (!n->myimms.bit.ms)
    {
      if ((n->myimms.bit.m == 0) && (n->imms.bit.m == 0) &&
	  (n->state == NEIGHBOR_EXCHANGE))
      {
	ospf_neigh_sm(n, INM_EXDONE);
      }
    }
    break;

  default:			/* Ignore it */
    break;
  }
}
