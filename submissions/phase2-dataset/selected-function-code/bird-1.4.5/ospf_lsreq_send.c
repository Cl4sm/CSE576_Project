ospf_lsreq_send(struct ospf_neighbor *n)
{
  snode *sn;
  struct top_hash_entry *en;
  struct ospf_lsreq_packet *pk;
  struct ospf_packet *op;
  struct ospf_lsreq_header *lsh;
  u16 length;
  int i, j;
  struct proto *p = &n->ifa->oa->po->proto;

  pk = ospf_tx_buffer(n->ifa);
  op = &pk->ospf_packet;

  ospf_pkt_fill_hdr(n->ifa, pk, LSREQ_P);

  sn = SHEAD(n->lsrql);
  if (EMPTY_SLIST(n->lsrql))
  {
    if (n->state == NEIGHBOR_LOADING)
      ospf_neigh_sm(n, INM_LOADDONE);
    return;
  }

  i = j = (ospf_pkt_maxsize(n->ifa) - sizeof(struct ospf_lsreq_packet)) /
    sizeof(struct ospf_lsreq_header);
  lsh = pk->lsh;

  for (; i > 0; i--)
  {
    en = (struct top_hash_entry *) sn;
    lsh->type = htonl(en->lsa.type);
    lsh->rt = htonl(en->lsa.rt);
    lsh->id = htonl(en->lsa.id);
    DBG("Requesting %uth LSA: Type: %u, ID: %R, RT: %R, SN: 0x%x, Age %u\n",
	i, en->lsa.type, en->lsa.id, en->lsa.rt, en->lsa.sn, en->lsa.age);
    lsh++;
    if (sn == STAIL(n->lsrql))
      break;
    sn = sn->next;
  }
  if (i != 0)
    i--;

  length =
    sizeof(struct ospf_lsreq_packet) + (j -
					i) * sizeof(struct ospf_lsreq_header);
  op->length = htons(length);

  OSPF_PACKET(ospf_dump_lsreq, pk, "LSREQ packet sent to %I via %s", n->ip, n->ifa->ifname);
  ospf_send_to(n->ifa, n->ip);
}
