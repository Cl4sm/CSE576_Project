ospf_hello_send(struct ospf_iface *ifa, int kind, struct ospf_neighbor *dirn)
{
  struct ospf_hello_packet *pkt;
  struct ospf_packet *op;
  struct proto *p;
  struct ospf_neighbor *neigh, *n1;
  u16 length;
  int i;
  struct nbma_node *nb;

  if (ifa->state <= OSPF_IS_LOOP)
    return;

  if (ifa->stub)
    return;			/* Don't send any packet on stub iface */

  p = (struct proto *) (ifa->oa->po);
  DBG("%s: Hello/Poll timer fired on interface %s with IP %I\n",
      p->name, ifa->ifname, ifa->addr->ip);

  /* Now we should send a hello packet */
  pkt = ospf_tx_buffer(ifa);
  op = &pkt->ospf_packet;

  /* Now fill ospf_hello header */
  ospf_pkt_fill_hdr(ifa, pkt, HELLO_P);

#ifdef OSPFv2
  pkt->netmask = ipa_mkmask(ifa->addr->pxlen);
  ipa_hton(pkt->netmask);
  if ((ifa->type == OSPF_IT_VLINK) ||
      ((ifa->type == OSPF_IT_PTP) && !ifa->ptp_netmask))
    pkt->netmask = IPA_NONE;
#endif

  pkt->helloint = ntohs(ifa->helloint);
  pkt->priority = ifa->priority;

#ifdef OSPFv3
  pkt->iface_id = htonl(ifa->iface_id);

  pkt->options3 = ifa->oa->options >> 16;
  pkt->options2 = ifa->oa->options >> 8;
#endif
  pkt->options = ifa->oa->options;

#ifdef OSPFv2
  pkt->deadint = htonl(ifa->deadint);
  pkt->dr = htonl(ipa_to_u32(ifa->drip));
  pkt->bdr = htonl(ipa_to_u32(ifa->bdrip));
#else /* OSPFv3 */
  pkt->deadint = htons(ifa->deadint);
  pkt->dr = htonl(ifa->drid);
  pkt->bdr = htonl(ifa->bdrid);
#endif

  /* Fill all neighbors */
  i = 0;

  if (kind != OHS_SHUTDOWN)
  {
    u32 *pp = (u32 *) (((u8 *) pkt) + sizeof(struct ospf_hello_packet));
    WALK_LIST(neigh, ifa->neigh_list)
    {
      if ((i+1) * sizeof(u32) + sizeof(struct ospf_hello_packet) > ospf_pkt_maxsize(ifa))
      {
	log(L_WARN "%s: Too many neighbors on interface %s", p->name, ifa->ifname);
	break;
      }
      *(pp + i) = htonl(neigh->rid);
      i++;
    }
  }

  length = sizeof(struct ospf_hello_packet) + i * sizeof(u32);
  op->length = htons(length);

  switch(ifa->type)
  {
  case OSPF_IT_BCAST:
  case OSPF_IT_PTP:
    ospf_send_to_all(ifa);
    break;

  case OSPF_IT_NBMA:
    if (dirn)		/* Response to received hello */
    {
      ospf_send_to(ifa, dirn->ip);
      break;
    }

    int to_all = ifa->state > OSPF_IS_DROTHER;
    int me_elig = ifa->priority > 0;
 
    if (kind == OHS_POLL)	/* Poll timer */
    {
      WALK_LIST(nb, ifa->nbma_list)
	if (!nb->found && (to_all || (me_elig && nb->eligible)))
	  ospf_send_to(ifa, nb->ip);
    }
    else			/* Hello timer */
    {
      WALK_LIST(n1, ifa->neigh_list)
	if (to_all || (me_elig && (n1->priority > 0)) ||
	    (n1->rid == ifa->drid) || (n1->rid == ifa->bdrid))
	  ospf_send_to(ifa, n1->ip);
    }
    break;

  case OSPF_IT_PTMP:
    WALK_LIST(n1, ifa->neigh_list)
      ospf_send_to(ifa, n1->ip);

    WALK_LIST(nb, ifa->nbma_list)
      if (!nb->found)
	ospf_send_to(ifa, nb->ip);

    /* If there is no other target, we also send HELLO packet to the other end */
    if (ipa_nonzero(ifa->addr->opposite) && !ifa->strictnbma &&
	EMPTY_LIST(ifa->neigh_list) && EMPTY_LIST(ifa->nbma_list))
      ospf_send_to(ifa, ifa->addr->opposite);
    break;

  case OSPF_IT_VLINK:
    ospf_send_to(ifa, ifa->vip);
    break;

  default:
    bug("Bug in ospf_hello_send()");
  }

  OSPF_TRACE(D_PACKETS, "HELLO packet sent via %s", ifa->ifname);
}
