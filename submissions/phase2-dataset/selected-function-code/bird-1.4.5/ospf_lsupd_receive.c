ospf_lsupd_receive(struct ospf_packet *ps_i, struct ospf_iface *ifa,
		   struct ospf_neighbor *n)
{

  struct ospf_neighbor *ntmp;
  struct proto_ospf *po = ifa->oa->po;
  struct proto *p = &po->proto;
  unsigned int i, max, sendreq = 1;

  unsigned int size = ntohs(ps_i->length);
  if (size < (sizeof(struct ospf_lsupd_packet) + sizeof(struct ospf_lsa_header)))
  {
    log(L_ERR "OSPF: Bad LSUPD packet from %I - too short (%u B)", n->ip, size);
    return;
  }

  struct ospf_lsupd_packet *ps = (void *) ps_i;
  OSPF_PACKET(ospf_dump_lsupd, ps, "LSUPD packet received from %I via %s", n->ip, ifa->ifname);

  if (n->state < NEIGHBOR_EXCHANGE)
  {
    OSPF_TRACE(D_PACKETS, "Received lsupd in lesser state than EXCHANGE from (%I)", n->ip);
    return;
  }

  ospf_neigh_sm(n, INM_HELLOREC);	/* Questionable */

  unsigned int offset = sizeof(struct ospf_lsupd_packet);
  unsigned int bound = size - sizeof(struct ospf_lsa_header);

  max = ntohl(ps->lsano);
  for (i = 0; i < max; i++)
  {
    struct ospf_lsa_header lsatmp;
    struct top_hash_entry *lsadb;

    if (offset > bound)
    {
      log(L_WARN "Received lsupd from %I is too short!", n->ip);
      ospf_neigh_sm(n, INM_BADLSREQ);
      return;
    }

    struct ospf_lsa_header *lsa = (void *) (((u8 *) ps) + offset);
    unsigned int lsalen = ntohs(lsa->length);
    offset += lsalen;
 
    if ((offset > size) || ((lsalen % 4) != 0) ||
	(lsalen <= sizeof(struct ospf_lsa_header)))
    {
      log(L_WARN "Received LSA from %I with bad length", n->ip);
      ospf_neigh_sm(n, INM_BADLSREQ);
      break;
    }

    /* pg 143 (1) */
    u16 chsum = lsa->checksum;
    if (chsum != lsasum_check(lsa, NULL))
    {
      log(L_WARN "Received bad lsa checksum from %I: %x %x", n->ip, chsum, lsa->checksum);
      continue;
    }

#ifdef OSPFv2
    /* pg 143 (2) */
    if ((lsa->type == 0) || (lsa->type == 6) || (lsa->type > LSA_T_NSSA))
    {
      log(L_WARN "Unknown LSA type from %I", n->ip);
      continue;
    }

    /* pg 143 (3) */
    if ((lsa->type == LSA_T_EXT) && !oa_is_ext(ifa->oa))
    {
      log(L_WARN "Received External LSA in stub area from %I", n->ip);
      continue;
    }
#else /* OSPFv3 */
    u16 scope = ntoht(lsa->type) & LSA_SCOPE_MASK;

    /* 4.5.1 (2) */
    if ((scope == LSA_SCOPE_AS) && !oa_is_ext(ifa->oa))
    {
      log(L_WARN "Received LSA with AS scope in stub area from %I", n->ip);
      continue;
    }

    /* 4.5.1 (3) */
    if (scope == LSA_SCOPE_RES)
    {
      log(L_WARN "Received LSA with invalid scope from %I", n->ip);
      continue;
    }
#endif

    ntohlsah(lsa, &lsatmp);

    DBG("Update Type: %u ID: %R RT: %R, Sn: 0x%08x Age: %u, Sum: %u\n",
	lsatmp.type, lsatmp.id, lsatmp.rt, lsatmp.sn, lsatmp.age, lsatmp.checksum);

    /* FIXME domain should be link id for unknown LSA types with zero Ubit */
    u32 domain = ospf_lsa_domain(lsatmp.type, ifa);
    lsadb = ospf_hash_find_header(po->gr, domain, &lsatmp);

#ifdef LOCAL_DEBUG
    if (lsadb)
      DBG("I have Type: %u ID: %R RT: %R, Sn: 0x%08x Age: %u, Sum: %u\n",
	  lsadb->lsa.type, lsadb->lsa.id, lsadb->lsa.rt,
	  lsadb->lsa.sn, lsadb->lsa.age, lsadb->lsa.checksum);
#endif

    /* pg 143 (4) */
    if ((lsatmp.age == LSA_MAXAGE) && (lsadb == NULL) && can_flush_lsa(po))
    {
      ospf_lsack_enqueue(n, lsa, ACKL_DIRECT);
      continue;
    }

    /* pg 144 (5) */
    if ((lsadb == NULL) || (lsa_comp(&lsatmp, &lsadb->lsa) == CMP_NEWER))
    {
      struct ospf_iface *ift = NULL;
      int self = (lsatmp.rt == po->router_id);

      DBG("PG143(5): Received LSA is newer\n");

#ifdef OSPFv2
      /* 13.4 - check self-originated LSAs of NET type */
      if ((!self) && (lsatmp.type == LSA_T_NET))
      {
	struct ospf_iface *nifa;
	WALK_LIST(nifa, po->iface_list)
	{
	  if (!nifa->iface)
	    continue;
	  if (ipa_equal(nifa->addr->ip, ipa_from_u32(lsatmp.id)))
	  {
	    self = 1;
	    break;
	  }
	}
      }
#endif

      /* pg 145 (5f) - premature aging of self originated lsa */
      if (self)
      {
	if ((lsatmp.age == LSA_MAXAGE) && (lsatmp.sn == LSA_MAXSEQNO))
	{
	  ospf_lsack_enqueue(n, lsa, ACKL_DIRECT);
	  continue;
	}

	OSPF_TRACE(D_EVENTS, "Received old self-originated LSA (Type: %04x, Id: %R, Rt: %R)",
		   lsatmp.type, lsatmp.id, lsatmp.rt);

	if (lsadb)
	{
	  OSPF_TRACE(D_EVENTS, "Reflooding new self-originated LSA with newer sequence number");
	  lsadb->lsa.sn = lsatmp.sn + 1;
	  lsadb->lsa.age = 0;
	  lsadb->inst_t = now;
	  lsadb->ini_age = 0;
	  lsasum_calculate(&lsadb->lsa, lsadb->lsa_body);
	  ospf_lsupd_flood(po, NULL, NULL, &lsadb->lsa, domain, 1);
	}
	else
	{
	  OSPF_TRACE(D_EVENTS, "Premature aging it");
	  lsatmp.age = LSA_MAXAGE;
	  lsatmp.sn = LSA_MAXSEQNO;
	  lsa->age = htons(LSA_MAXAGE);
	  lsa->sn = htonl(LSA_MAXSEQNO);
	  lsasum_check(lsa, (lsa + 1));	/* It also calculates chsum! */
	  lsatmp.checksum = ntohs(lsa->checksum);
	  ospf_lsupd_flood(po, NULL, lsa, &lsatmp, domain, 0);
	}
	continue;
      }

      /* pg 144 (5a) */
      if (lsadb && ((now - lsadb->inst_t) <= MINLSARRIVAL))	/* FIXME: test for flooding? */
      {
	OSPF_TRACE(D_EVENTS, "Skipping LSA received in less that MINLSARRIVAL");
	sendreq = 0;
	continue;
      }

      /* Remove old from all ret lists */
      /* pg 144 (5c) */
      /* Must be done before (5b), otherwise it also removes the new entries from (5b) */
      if (lsadb)
	WALK_LIST(ift, po->iface_list)
	  WALK_LIST(ntmp, ift->neigh_list)
      {
	struct top_hash_entry *en;
	if (ntmp->state > NEIGHBOR_EXSTART)
	  if ((en = ospf_hash_find_header(ntmp->lsrth, domain, &lsadb->lsa)) != NULL)
	  {
	    s_rem_node(SNODE en);
	    ospf_hash_delete(ntmp->lsrth, en);
	  }
      }

      /* pg 144 (5b) */
      if (ospf_lsupd_flood(po, n, lsa, &lsatmp, domain, 1) == 0)
      {
	DBG("Wasn't flooded back\n");	/* ps 144(5e), pg 153 */
	if (ifa->state == OSPF_IS_BACKUP)
	{
	  if (ifa->drid == n->rid)
	    ospf_lsack_enqueue(n, lsa, ACKL_DELAY);
	}
	else
	  ospf_lsack_enqueue(n, lsa, ACKL_DELAY);
      }

      if ((lsatmp.age == LSA_MAXAGE) && (lsatmp.sn == LSA_MAXSEQNO)
	  && lsadb && can_flush_lsa(po))
      {
	flush_lsa(lsadb, po);
	schedule_rtcalc(po);
	continue;
      }				/* FIXME lsack? */

      /* pg 144 (5d) */
      void *body = mb_alloc(p->pool, lsatmp.length - sizeof(struct ospf_lsa_header));
      ntohlsab(lsa + 1, body, lsatmp.length - sizeof(struct ospf_lsa_header));

      /* We will do validation check after flooding and
	 acknowledging given LSA to minimize problems
	 when communicating with non-validating peer */
      if (lsa_validate(&lsatmp, body) == 0)
      {
	log(L_WARN "Received invalid LSA from %I", n->ip);
	mb_free(body);
	continue;	
      }

      lsadb = lsa_install_new(po, &lsatmp, domain, body);
      DBG("New LSA installed in DB\n");

#ifdef OSPFv3
      /* Events 6,7 from RFC5340 4.4.3. */
      if ((lsatmp.type == LSA_T_LINK) &&
	  (ifa->state == OSPF_IS_DR))
	schedule_net_lsa(ifa);
#endif

      continue;
    }

    /* FIXME pg145 (6) */

    /* pg145 (7) */
    if (lsa_comp(&lsatmp, &lsadb->lsa) == CMP_SAME)
    {
      struct top_hash_entry *en;
      DBG("PG145(7) Got the same LSA\n");
      if ((en = ospf_hash_find_header(n->lsrth, lsadb->domain, &lsadb->lsa)) != NULL)
      {
	/* pg145 (7a) */
	s_rem_node(SNODE en);
	ospf_hash_delete(n->lsrth, en);

	if (ifa->state == OSPF_IS_BACKUP)
	{
	  if (n->rid == ifa->drid)
	    ospf_lsack_enqueue(n, lsa, ACKL_DELAY);
	}
      }
      else
      {
	/* pg145 (7b) */
	ospf_lsack_enqueue(n, lsa, ACKL_DIRECT);
      }
      sendreq = 0;
      continue;
    }

    /* pg145 (8) */
    if ((lsadb->lsa.age == LSA_MAXAGE) && (lsadb->lsa.sn == LSA_MAXSEQNO))
    {
      continue;
    }

    {
      list l;
      struct l_lsr_head ll;
      init_list(&l);
      ll.lsh.id = lsadb->lsa.id;
      ll.lsh.rt = lsadb->lsa.rt;
      ll.lsh.type = lsadb->lsa.type;
      add_tail(&l, NODE & ll);
      ospf_lsupd_send_list(n, &l);
    }
  }

  /* Send direct LSAs */
  ospf_lsack_send(n, ACKL_DIRECT);

  if (sendreq && (n->state == NEIGHBOR_LOADING))
  {
    ospf_lsreq_send(n);		/* Ask for another part of neighbor's database */
  }
}
