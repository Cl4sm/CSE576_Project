ospf_rt_spfa_rtlinks(struct ospf_area *oa, struct top_hash_entry *act, struct top_hash_entry *en)
{
  // struct proto *p = &oa->po->proto;
  struct proto_ospf *po = oa->po;
  ip_addr prefix UNUSED;
  int pxlen UNUSED, i;

  struct ospf_lsa_rt *rt = en->lsa_body;
  struct ospf_lsa_rt_link *rr = (struct ospf_lsa_rt_link *) (rt + 1);

  for (i = 0; i < lsa_rt_count(&en->lsa); i++)
    {
      struct ospf_lsa_rt_link *rtl = rr + i;
      struct top_hash_entry *tmp = NULL;

      DBG("     Working on link: %R (type: %u)  ", rtl->id, rtl->type);
      switch (rtl->type)
	{
#ifdef OSPFv2
	case LSART_STUB:
	  /*
	   * RFC 2328 in 16.1. (2a) says to handle stub networks in an
	   * second phase after the SPF for an area is calculated. We get
	   * the same result by handing them here because add_network()
	   * will keep the best (not the first) found route.
	   */
	  prefix = ipa_from_u32(rtl->id & rtl->data);
	  pxlen = ipa_mklen(ipa_from_u32(rtl->data));
	  add_network(oa, prefix, pxlen, act->dist + rtl->metric, act, i);
	  break;
#endif

	case LSART_NET:
#ifdef OSPFv2
	  /* In OSPFv2, rtl->id is IP addres of DR, Router ID is not known */
	  tmp = ospf_hash_find_net(po->gr, oa->areaid, rtl->id);
#else /* OSPFv3 */
	  tmp = ospf_hash_find(po->gr, oa->areaid, rtl->nif, rtl->id, LSA_T_NET);
#endif
	  break;

	case LSART_VLNK:
	case LSART_PTP:
	  tmp = ospf_hash_find_rt(po->gr, oa->areaid, rtl->id);
	  break;

	default:
	  log("Unknown link type in router lsa. (rid = %R)", act->lsa.id);
	  break;
	}

      if (tmp)
	DBG("Going to add cand, Mydist: %u, Req: %u\n",
	    tmp->dist, act->dist + rtl->metric);
      add_cand(&oa->cand, tmp, act, act->dist + rtl->metric, oa, i);
    }
}
