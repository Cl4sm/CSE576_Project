link_back(struct ospf_area *oa, struct top_hash_entry *en, struct top_hash_entry *par)
{
  u32 i, *rts;
  struct ospf_lsa_net *ln;
  struct ospf_lsa_rt *rt;
  struct ospf_lsa_rt_link *rtl, *rr;
  struct top_hash_entry *tmp;
  struct proto_ospf *po = oa->po;

  if (!en || !par) return 0;

  /* We should check whether there is a link back from en to par,
     this is used in SPF calc (RFC 2328 16.1. (2b)). According to RFC 2328
     note 23, we don't have to find the same link that is used for par
     to en, any link is enough. This we do for ptp links. For net-rt
     links, we have to find the same link to compute proper lb/lb_id,
     which may be later used as the next hop. */

  /* In OSPFv2, en->lb is set here. In OSPFv3, en->lb is just cleared here,
     it is set in process_prefixes() to any global addres in the area */

  en->lb = IPA_NONE;
#ifdef OSPFv3
  en->lb_id = 0;
#endif
  switch (en->lsa.type)
  {
    case LSA_T_RT:
      rt = (struct ospf_lsa_rt *) en->lsa_body;
      rr = (struct ospf_lsa_rt_link *) (rt + 1);
      for (i = 0; i < lsa_rt_count(&en->lsa); i++)
      {
	rtl = (rr + i);
	switch (rtl->type)
	{
	case LSART_STUB:
	  break;
	case LSART_NET:
#ifdef OSPFv2
	  /* In OSPFv2, rtl->id is IP addres of DR, Router ID is not known */
	  tmp = ospf_hash_find_net(po->gr, oa->areaid, rtl->id);
#else /* OSPFv3 */
	  tmp = ospf_hash_find(po->gr, oa->areaid, rtl->nif, rtl->id, LSA_T_NET);
#endif
	  if (tmp == par)
	  {
#ifdef OSPFv2
	    en->lb = ipa_from_u32(rtl->data);
#else /* OSPFv3 */
	    en->lb_id = rtl->lif;
#endif
	    return 1;
	  }

	  break;
	case LSART_VLNK:
	case LSART_PTP:
	  /* Not necessary the same link, see RFC 2328 [23] */
	  tmp = ospf_hash_find_rt(po->gr, oa->areaid, rtl->id);
	  if (tmp == par)
            return 1;

	  break;
	default:
	  log(L_WARN "Unknown link type in router lsa. (rid = %R)", en->lsa.rt);
	  break;
	}
      }
      break;
    case LSA_T_NET:
      ln = en->lsa_body;
      rts = (u32 *) (ln + 1);
      for (i = 0; i < lsa_net_count(&en->lsa); i++)
      {
	tmp = ospf_hash_find_rt(po->gr, oa->areaid, rts[i]);
	if (tmp == par)
          return 1;
      }
      break;
    default:
      bug("Unknown lsa type %x.", en->lsa.type);
  }
  return 0;
}
