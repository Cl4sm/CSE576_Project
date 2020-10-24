{
  struct ospf_packet *op = &pkt->ospf_packet;

  ASSERT(op->type == DBDES_P);
  ospf_dump_common(p, op);
  log(L_TRACE "%s:     imms     %s%s%s",
      p->name, pkt->imms.bit.ms ? "MS " : "",
      pkt->imms.bit.m ? "M " : "",
      pkt->imms.bit.i ? "I " : "" );
  log(L_TRACE "%s:     ddseq    %u", p->name, ntohl(pkt->ddseq));

  struct ospf_lsa_header *plsa = (void *) (pkt + 1);
  unsigned int i, j;

  j = (ntohs(op->length) - sizeof(struct ospf_dbdes_packet)) /
    sizeof(struct ospf_lsa_header);

  for (i = 0; i < j; i++)
    ospf_dump_lsahdr(p, plsa + i);
}
