void
ospf_lsack_receive(struct ospf_packet *ps_i, struct ospf_iface *ifa,
		   struct ospf_neighbor *n)
{
  struct proto *p = &ifa->oa->po->proto;
  struct ospf_lsa_header lsa;
  struct top_hash_entry *en;
  unsigned int i, lsano;

  unsigned int size = ntohs(ps_i->length);
  if (size < sizeof(struct ospf_lsack_packet))
  {
    log(L_ERR "Bad OSPF LSACK packet from %I -  too short (%u B)", n->ip, size);
    return;
  }

  struct ospf_lsack_packet *ps = (void *) ps_i;
  OSPF_PACKET(ospf_dump_lsack, ps, "LSACK packet received from %I via %s", n->ip, ifa->ifname);

  ospf_neigh_sm(n, INM_HELLOREC);

  if (n->state < NEIGHBOR_EXCHANGE)
    return;

  lsano = (size - sizeof(struct ospf_lsack_packet)) /
    sizeof(struct ospf_lsa_header);
  for (i = 0; i < lsano; i++)
  {
    ntohlsah(ps->lsh + i, &lsa);
    u32 dom = ospf_lsa_domain(lsa.type, n->ifa);
    if ((en = ospf_hash_find_header(n->lsrth, dom, &lsa)) == NULL)
      continue;			/* pg 155 */

    if (lsa_comp(&lsa, &en->lsa) != CMP_SAME)	/* pg 156 */
    {
      if ((lsa.sn == LSA_MAXSEQNO) && (lsa.age == LSA_MAXAGE))
	continue;

      OSPF_TRACE(D_PACKETS, "Strange LSACK from %I", n->ip);
      OSPF_TRACE(D_PACKETS, "Type: %04x, Id: %R, Rt: %R",
		 lsa.type, lsa.id, lsa.rt);
      OSPF_TRACE(D_PACKETS, "I have: Age: %4u, Seq: %08x, Sum: %04x",
		 en->lsa.age, en->lsa.sn, en->lsa.checksum);
      OSPF_TRACE(D_PACKETS, "He has: Age: %4u, Seq: %08x, Sum: %04x",
		 lsa.age, lsa.sn, lsa.checksum);
      continue;
    }

    DBG("Deleting LS Id: %R RT: %R Type: %u from LS Retl for neighbor %R\n",
	lsa.id, lsa.rt, lsa.type, n->rid);
    s_rem_node(SNODE en);
    ospf_hash_delete(n->lsrth, en);
  }
}
