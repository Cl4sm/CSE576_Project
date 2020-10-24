ospf_age(struct proto_ospf *po)
{
  struct proto *p = &po->proto;
  struct top_hash_entry *en, *nxt;
  int flush = can_flush_lsa(po);

  WALK_SLIST_DELSAFE(en, nxt, po->lsal)
  {
    if (en->lsa.age == LSA_MAXAGE)
    {
      if (flush)
	flush_lsa(en, po);
      continue;
    }
    if ((en->lsa.rt == po->router_id) && (en->lsa.age >= LSREFRESHTIME))
    {
      OSPF_TRACE(D_EVENTS, "Refreshing my LSA: Type: %u, Id: %R, Rt: %R",
		 en->lsa.type, en->lsa.id, en->lsa.rt);
      en->lsa.sn++;
      en->lsa.age = 0;
      en->inst_t = now;
      en->ini_age = 0;
      lsasum_calculate(&en->lsa, en->lsa_body);
      ospf_lsupd_flood(po, NULL, NULL, &en->lsa, en->domain, 1);
      continue;
    }
    if ((en->lsa.age = (en->ini_age + (now - en->inst_t))) >= LSA_MAXAGE)
    {
      if (flush)
      {
	flush_lsa(en, po);
	schedule_rtcalc(po);
      }
      else
	en->lsa.age = LSA_MAXAGE;
    }
  }
}
