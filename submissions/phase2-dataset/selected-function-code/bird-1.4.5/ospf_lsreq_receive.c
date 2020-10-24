void
ospf_lsreq_receive(struct ospf_packet *ps_i, struct ospf_iface *ifa,
		   struct ospf_neighbor *n)
{
  struct ospf_area *oa = ifa->oa;
  struct proto_ospf *po = oa->po;
  struct proto *p = &po->proto;
  struct ospf_lsreq_header *lsh;
  struct l_lsr_head *llsh;
  list uplist;
  slab *upslab;
  int i, lsano;

  unsigned int size = ntohs(ps_i->length);
  if (size < sizeof(struct ospf_lsreq_packet))
  {
    log(L_ERR "Bad OSPF LSREQ packet from %I -  too short (%u B)", n->ip, size);
    return;
  }

  struct ospf_lsreq_packet *ps = (void *) ps_i;
  OSPF_PACKET(ospf_dump_lsreq, ps, "LSREQ packet received from %I via %s", n->ip, ifa->ifname);

  if (n->state < NEIGHBOR_EXCHANGE)
    return;

  ospf_neigh_sm(n, INM_HELLOREC);

  lsh = ps->lsh;
  init_list(&uplist);
  upslab = sl_new(n->pool, sizeof(struct l_lsr_head));

  lsano = (size - sizeof(struct ospf_lsreq_packet)) /
    sizeof(struct ospf_lsreq_header);
  for (i = 0; i < lsano; lsh++, i++)
  {
    u32 hid = ntohl(lsh->id);
    u32 hrt = ntohl(lsh->rt);
    u32 htype = ntohl(lsh->type);
    u32 dom = ospf_lsa_domain(htype, ifa);
    DBG("Processing requested LSA: Type: %u, ID: %R, RT: %R\n", lsh->type, hid, hrt);
    llsh = sl_alloc(upslab);
    llsh->lsh.id = hid;
    llsh->lsh.rt = hrt;
    llsh->lsh.type = htype;
    add_tail(&uplist, NODE llsh);
    if (ospf_hash_find(po->gr, dom, hid, hrt, htype) == NULL)
    {
      log(L_WARN "Received bad LSREQ from %I: Type: %04x, Id: %R, Rt: %R",
	  n->ip, htype, hid, hrt);
      ospf_neigh_sm(n, INM_BADLSREQ);
      rfree(upslab);
      return;
    }
  }
  ospf_lsupd_send_list(n, &uplist);
  rfree(upslab);
}
