void				/* I send all I received in LSREQ */
ospf_lsupd_send_list(struct ospf_neighbor *n, list * l)
{
  struct ospf_area *oa = n->ifa->oa;
  struct proto *p = &oa->po->proto;
  struct l_lsr_head *lsr;
  struct top_hash_entry *en;
  struct ospf_lsupd_packet *pkt;
  u32 len, len2, lsano;
  char *buf;

  pkt = ospf_tx_buffer(n->ifa);
  buf = (void *) pkt;

  lsr = HEAD(*l);
  while(NODE_NEXT(lsr))
  {
    /* Prepare the packet */
    ospf_pkt_fill_hdr(n->ifa, pkt, LSUPD_P);
    len = sizeof(struct ospf_lsupd_packet);
    lsano = 0;

    /* Fill the packet with LSAs */
    while(NODE_NEXT(lsr))
    {
      u32 domain = ospf_lsa_domain(lsr->lsh.type, n->ifa);
      en = ospf_hash_find(oa->po->gr, domain, lsr->lsh.id, lsr->lsh.rt, lsr->lsh.type);
      if (en == NULL)
      {
	/* Probably flushed LSA, this should not happen */
	// log(L_WARN "OSPF: LSA disappeared (Type: %04x, Id: %R, Rt: %R)", lsr->lsh.type, lsr->lsh.id, lsr->lsh.rt);
	lsr = NODE_NEXT(lsr);
	continue;			
      }

      len2 = len + en->lsa.length;
      if (len2 > ospf_pkt_maxsize(n->ifa))
      {
	/* The packet if full, stop adding LSAs and sent it */
	if (lsano > 0)
	  break;

	/* LSA is larger than MTU, check buffer size */
	if (ospf_iface_assure_bufsize(n->ifa, len2) < 0)
	{
	  /* Cannot fit in a tx buffer, skip that */
	  log(L_ERR "OSPF: LSA too large to send (Type: %04x, Id: %R, Rt: %R)", 
	      lsr->lsh.type, lsr->lsh.id, lsr->lsh.rt);
	  lsr = NODE_NEXT(lsr);
	  continue;
	}

	/* TX buffer could be reallocated */
	pkt = ospf_tx_buffer(n->ifa);
	buf = (void *) pkt;
      }

      /* Copy the LSA to the packet */
      htonlsah(&(en->lsa), (struct ospf_lsa_header *) (buf + len));
      htonlsab(en->lsa_body, buf + len + sizeof(struct ospf_lsa_header),
	       en->lsa.length - sizeof(struct ospf_lsa_header));
      len = len2;
      lsano++;
      lsr = NODE_NEXT(lsr);
    }

    if (lsano == 0)
      break;

    /* Send the packet */
    pkt->lsano = htonl(lsano);
    pkt->ospf_packet.length = htons(len);
    OSPF_PACKET(ospf_dump_lsupd, pkt, "LSUPD packet sent to %I via %s",
		n->ip, n->ifa->ifname);
    ospf_send_to(n->ifa, n->ip);
  }
}
