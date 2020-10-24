void
ospf_lsack_send(struct ospf_neighbor *n, int queue)
{
  struct ospf_packet *op;
  struct ospf_lsack_packet *pk;
  u16 len, i = 0;
  struct ospf_lsa_header *h;
  struct lsah_n *no;
  struct ospf_iface *ifa = n->ifa;
  struct proto *p = &n->ifa->oa->po->proto;

  if (EMPTY_LIST(n->ackl[queue]))
    return;

  pk = ospf_tx_buffer(ifa);
  op = &pk->ospf_packet;

  ospf_pkt_fill_hdr(n->ifa, pk, LSACK_P);
  h = pk->lsh;

  while (!EMPTY_LIST(n->ackl[queue]))
  {
    no = (struct lsah_n *) HEAD(n->ackl[queue]);
    memcpy(h + i, &no->lsa, sizeof(struct ospf_lsa_header));
    DBG("Iter %u ID: %R, RT: %R, Type: %04x\n", i, ntohl((h + i)->id),
	ntohl((h + i)->rt), (h + i)->type);
    i++;
    rem_node(NODE no);
    mb_free(no);
    if ((i * sizeof(struct ospf_lsa_header) +
	 sizeof(struct ospf_lsack_packet)) > ospf_pkt_maxsize(n->ifa))
    {
      if (!EMPTY_LIST(n->ackl[queue]))
      {
	len =
	  sizeof(struct ospf_lsack_packet) +
	  i * sizeof(struct ospf_lsa_header);
	op->length = htons(len);
	DBG("Sending and continuing! Len=%u\n", len);

	OSPF_PACKET(ospf_dump_lsack, pk, "LSACK packet sent via %s", ifa->ifname);

	if (ifa->type == OSPF_IT_BCAST)
	{
	  if ((ifa->state == OSPF_IS_DR) || (ifa->state == OSPF_IS_BACKUP))
	    ospf_send_to_all(ifa);
	  else if (ifa->cf->real_bcast)
	    ospf_send_to_bdr(ifa);
	  else
	    ospf_send_to(ifa, AllDRouters);
	}
	else
	{
	  if ((ifa->state == OSPF_IS_DR) || (ifa->state == OSPF_IS_BACKUP))
	    ospf_send_to_agt(ifa, NEIGHBOR_EXCHANGE);
	  else
	    ospf_send_to_bdr(ifa);
	}

	ospf_pkt_fill_hdr(n->ifa, pk, LSACK_P);
	i = 0;
      }
    }
  }

  len = sizeof(struct ospf_lsack_packet) + i * sizeof(struct ospf_lsa_header);
  op->length = htons(len);
  DBG("Sending! Len=%u\n", len);

  OSPF_PACKET(ospf_dump_lsack, pk, "LSACK packet sent via %s", ifa->ifname);

  if (ifa->type == OSPF_IT_BCAST)
  {
    if ((ifa->state == OSPF_IS_DR) || (ifa->state == OSPF_IS_BACKUP))
      ospf_send_to_all(ifa);
    else if (ifa->cf->real_bcast)
      ospf_send_to_bdr(ifa);
    else
      ospf_send_to(ifa, AllDRouters);
  }
  else
    ospf_send_to_agt(ifa, NEIGHBOR_EXCHANGE);
}
