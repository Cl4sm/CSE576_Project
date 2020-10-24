static void ospf_dump_lsupd(struct proto *p, struct ospf_lsupd_packet *pkt)
{
  struct ospf_packet *op = &pkt->ospf_packet;

  ASSERT(op->type == LSUPD_P);
  ospf_dump_common(p, op);

  /* We know that ntohs(op->length) >= sizeof(struct ospf_lsa_header) */
  u8 *pbuf= (u8 *) pkt;
  unsigned int offset = sizeof(struct ospf_lsupd_packet);
  unsigned int bound = ntohs(op->length) - sizeof(struct ospf_lsa_header);
  unsigned int i, j, lsalen;

  j = ntohl(pkt->lsano);
  for (i = 0; i < j; i++)
    {
      if (offset > bound)
	{
	  log(L_TRACE "%s:     LSA      invalid", p->name);
	  return;
	}

      struct ospf_lsa_header *lsa = (void *) (pbuf + offset);
      ospf_dump_lsahdr(p, lsa);
      lsalen = ntohs(lsa->length);
      offset += lsalen;

      if (((lsalen % 4) != 0) || (lsalen <= sizeof(struct ospf_lsa_header)))
	{
	  log(L_TRACE "%s:     LSA      invalid", p->name);
	  return;
	}
    }
}
