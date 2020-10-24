lsasum_calculate(struct ospf_lsa_header *h, void *body)
{
  u16 length = h->length;

  //  log(L_WARN "Checksum %R %R %d start (len %d)", h->id, h->rt, h->type, length);
  htonlsah(h, h);
  htonlsab1(body, length - sizeof(struct ospf_lsa_header));

  /*
  char buf[1024];
  memcpy(buf, h, sizeof(struct ospf_lsa_header));
  memcpy(buf + sizeof(struct ospf_lsa_header), body, length - sizeof(struct ospf_lsa_header));
  buf_dump("CALC", buf, length);
  */

  (void) lsasum_check(h, body);

  //  log(L_WARN "Checksum result %4x", h->checksum);

  ntohlsah(h, h);
  ntohlsab1(body, length - sizeof(struct ospf_lsa_header));
}
