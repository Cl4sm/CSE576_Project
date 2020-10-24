int
ospf_lsupd_flood(struct proto_ospf *po,
		 struct ospf_neighbor *n, struct ospf_lsa_header *hn,
		 struct ospf_lsa_header *hh, u32 domain, int rtl)
{
  struct ospf_iface *ifa;
  struct ospf_neighbor *nn;
  struct top_hash_entry *en;
  struct proto *p = &po->proto;
  int ret, retval = 0;

  /* pg 148 */
  WALK_LIST(ifa, po->iface_list)
  {
    if (ifa->stub)
      continue;

    if (! ospf_lsa_flooding_allowed(hh, domain, ifa))
      continue;

    DBG("Wanted to flood LSA: Type: %u, ID: %R, RT: %R, SN: 0x%x, Age %u\n",
	hh->type, hh->id, hh->rt, hh->sn, hh->age);

    ret = 0;
    WALK_LIST(nn, ifa->neigh_list)
    {
      /* 13.3 (1a) */
      if (nn->state < NEIGHBOR_EXCHANGE)
	continue;

      /* 13.3 (1b) */
      if (nn->state < NEIGHBOR_FULL)
      {
	if ((en = ospf_hash_find_header(nn->lsrqh, domain, hh)) != NULL)
	{
	  DBG("That LSA found in lsreq list for neigh %R\n", nn->rid);

	  switch (lsa_comp(hh, &en->lsa))
	  {
	  case CMP_OLDER:
	    continue;
	    break;
	  case CMP_SAME:
	    s_rem_node(SNODE en);
	    if (en->lsa_body != NULL)
	      mb_free(en->lsa_body);
	    en->lsa_body = NULL;
	    DBG("Removing from lsreq list for neigh %R\n", nn->rid);
	    ospf_hash_delete(nn->lsrqh, en);
	    if ((EMPTY_SLIST(nn->lsrql)) && (nn->state == NEIGHBOR_LOADING))
	      ospf_neigh_sm(nn, INM_LOADDONE);
	    continue;
	    break;
	  case CMP_NEWER:
	    s_rem_node(SNODE en);
	    if (en->lsa_body != NULL)
	      mb_free(en->lsa_body);
	    en->lsa_body = NULL;
	    DBG("Removing from lsreq list for neigh %R\n", nn->rid);
	    ospf_hash_delete(nn->lsrqh, en);
	    if ((EMPTY_SLIST(nn->lsrql)) && (nn->state == NEIGHBOR_LOADING))
	      ospf_neigh_sm(nn, INM_LOADDONE);
	    break;
	  default:
	    bug("Bug in lsa_comp?");
	  }
	}
      }

      /* 13.3 (1c) */
      if (nn == n)
	continue;

      /* 13.3 (1d) */
      if (rtl)
      {
	/* In OSPFv3, there should be check whether receiving router understand
	   that type of LSA (for LSA types with U-bit == 0). But as we does not support
	   any optional LSA types, this is not needed yet */

	if ((en = ospf_hash_find_header(nn->lsrth, domain, hh)) == NULL)
	{
	  en = ospf_hash_get_header(nn->lsrth, domain, hh);
	}
	else
	{
	  s_rem_node(SNODE en);
	}
	s_add_tail(&nn->lsrtl, SNODE en);
	memcpy(&en->lsa, hh, sizeof(struct ospf_lsa_header));
	DBG("Adding that LSA for flood to %I\n", nn->ip);
      }
      else
      {
	if ((en = ospf_hash_find_header(nn->lsrth, domain, hh)) != NULL)
	{
	  s_rem_node(SNODE en);
	  ospf_hash_delete(nn->lsrth, en);
	}
      }

      ret = 1;
    }

    if (ret == 0)
      continue;			/* pg 150 (2) */

    if (n && (n->ifa == ifa))
    {
      if ((n->rid == ifa->drid) || n->rid == ifa->bdrid)
	continue;		/* pg 150 (3) */
      if (ifa->state == OSPF_IS_BACKUP)
	continue;		/* pg 150 (4) */
      retval = 1;
    }

    {
      u16 len, age;
      struct ospf_lsupd_packet *pk;
      struct ospf_packet *op;
      struct ospf_lsa_header *lh;

      /* Check iface buffer size */
      uint len2 = sizeof(struct ospf_lsupd_packet) + (hn ? ntohs(hn->length) : hh->length);
      if (ospf_iface_assure_bufsize(ifa, len2) < 0)
      {
	/* Cannot fit in a tx buffer, skip that iface */
	log(L_ERR "OSPF: LSA too large to flood on %s (Type: %04x, Id: %R, Rt: %R)", 
	    ifa->ifname, hh->type, hh->id, hh->rt);
	continue;
      }

      pk = ospf_tx_buffer(ifa);
      op = &pk->ospf_packet;

      ospf_pkt_fill_hdr(ifa, pk, LSUPD_P);
      pk->lsano = htonl(1);

      lh = (struct ospf_lsa_header *) (pk + 1);

      /* Copy LSA into the packet */
      if (hn)
      {
	memcpy(lh, hn, ntohs(hn->length));
      }
      else
      {
	u8 *help;
	struct top_hash_entry *en;

	htonlsah(hh, lh);
	help = (u8 *) (lh + 1);
	en = ospf_hash_find_header(po->gr, domain, hh);
	htonlsab(en->lsa_body, help, hh->length - sizeof(struct ospf_lsa_header));
      }

      len = sizeof(struct ospf_lsupd_packet) + ntohs(lh->length);

      age = ntohs(lh->age);
      age += ifa->inftransdelay;
      if (age > LSA_MAXAGE)
	age = LSA_MAXAGE;
      lh->age = htons(age);

      op->length = htons(len);

      OSPF_PACKET(ospf_dump_lsupd, pk, "LSUPD packet flooded via %s", ifa->ifname);

      switch (ifa->type)
      {
      case OSPF_IT_BCAST:
	if ((ifa->state == OSPF_IS_BACKUP) || (ifa->state == OSPF_IS_DR))
	  ospf_send_to_all(ifa);
	else if (ifa->cf->real_bcast)
	  ospf_send_to_bdr(ifa);
	else
	  ospf_send_to(ifa, AllDRouters);
	break;

      case OSPF_IT_NBMA:
	if ((ifa->state == OSPF_IS_BACKUP) || (ifa->state == OSPF_IS_DR))
	  ospf_send_to_agt(ifa, NEIGHBOR_EXCHANGE);
	else
	  ospf_send_to_bdr(ifa);
	break;

      case OSPF_IT_PTP:
	ospf_send_to_all(ifa);
	break;

      case OSPF_IT_PTMP:
	ospf_send_to_agt(ifa, NEIGHBOR_EXCHANGE);
	break;

      case OSPF_IT_VLINK:
	ospf_send_to(ifa, ifa->vip);
	break;

      default:
	bug("Bug in ospf_lsupd_flood()");
      }
    }
  }
  return retval;
}
