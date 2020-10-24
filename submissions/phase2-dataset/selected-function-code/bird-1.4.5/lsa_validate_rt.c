static int
lsa_validate_rt(struct ospf_lsa_header *lsa, struct ospf_lsa_rt *body)
{
  unsigned int i, max;

  if (lsa->length < (HDRLEN + sizeof(struct ospf_lsa_rt)))
    return 0;

  struct ospf_lsa_rt_link *rtl = (struct ospf_lsa_rt_link *) (body + 1);
  max = lsa_rt_count(lsa);

#ifdef OSPFv2
  if (body->links != max)
    return 0;
#endif  

  for (i = 0; i < max; i++)
  {
    u8 type = rtl[i].type;
    if (!((type == LSART_PTP) ||
	  (type == LSART_NET) ||
#ifdef OSPFv2
	  (type == LSART_STUB) ||
#endif
	  (type == LSART_VLNK)))
      return 0;
  }
  return 1;
}
