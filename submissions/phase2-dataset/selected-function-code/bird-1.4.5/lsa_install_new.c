lsa_install_new(struct proto_ospf *po, struct ospf_lsa_header *lsa, u32 domain, void *body)
{
  /* LSA can be temporarrily, but body must be mb_allocated. */
  int change = 0;
  struct top_hash_entry *en;

  if ((en = ospf_hash_find_header(po->gr, domain, lsa)) == NULL)
  {
    en = ospf_hash_get_header(po->gr, domain, lsa);
    change = 1;
  }
  else
  {
    if ((en->lsa.length != lsa->length)
#ifdef OSPFv2       
	|| (en->lsa.options != lsa->options)
#endif
	|| (en->lsa.age == LSA_MAXAGE)
	|| (lsa->age == LSA_MAXAGE)
	|| memcmp(en->lsa_body, body, lsa->length - sizeof(struct ospf_lsa_header)))
      change = 1;

    s_rem_node(SNODE en);
  }

  DBG("Inst lsa: Id: %R, Rt: %R, Type: %u, Age: %u, Sum: %u, Sn: 0x%x\n",
      lsa->id, lsa->rt, lsa->type, lsa->age, lsa->checksum, lsa->sn);

  s_add_tail(&po->lsal, SNODE en);
  en->inst_t = now;
  if (en->lsa_body != NULL)
    mb_free(en->lsa_body);
  en->lsa_body = body;
  memcpy(&en->lsa, lsa, sizeof(struct ospf_lsa_header));
  en->ini_age = en->lsa.age;

  if (change)
    schedule_rtcalc(po);

  return en;
}
