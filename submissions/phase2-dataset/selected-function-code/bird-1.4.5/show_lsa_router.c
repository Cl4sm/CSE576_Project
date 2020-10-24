show_lsa_router(struct proto_ospf *po, struct top_hash_entry *he, int first, int verbose)
{
  struct ospf_lsa_header *lsa = &(he->lsa);
  struct ospf_lsa_rt *rt = he->lsa_body;
  struct ospf_lsa_rt_link *rr = (struct ospf_lsa_rt_link *) (rt + 1);
  int max = lsa_rt_count(lsa);
  int i;

  if (first)
  {
    cli_msg(-1016, "");
    cli_msg(-1016, "\trouter %R", he->lsa.rt);
    show_lsa_distance(he);
  }


  for (i = 0; i < max; i++)
    if (rr[i].type == LSART_VLNK)
      cli_msg(-1016, "\t\tvlink %R metric %u", rr[i].id, rr[i].metric);

  for (i = 0; i < max; i++)
    if (rr[i].type == LSART_PTP)
      cli_msg(-1016, "\t\trouter %R metric %u", rr[i].id, rr[i].metric);

  for (i = 0; i < max; i++)
    if (rr[i].type == LSART_NET)
    {
#ifdef OSPFv2
      struct top_hash_entry *net_he = ospf_hash_find_net(po->gr, he->domain, rr[i].id);

      if (net_he)
      {
	struct ospf_lsa_header *net_lsa = &(net_he->lsa);
	struct ospf_lsa_net *net_ln = net_he->lsa_body;

	cli_msg(-1016, "\t\tnetwork %I/%d metric %u", 
		ipa_and(ipa_from_u32(net_lsa->id), net_ln->netmask),
		ipa_mklen(net_ln->netmask), rr[i].metric);
      }
      else
	cli_msg(-1016, "\t\tnetwork [%R] metric %u", rr[i].id, rr[i].metric);

#else /* OSPFv3 */
      cli_msg(-1016, "\t\tnetwork [%R-%u] metric %u", rr[i].id, rr[i].nif, rr[i].metric);
#endif
    }

#ifdef OSPFv2
  if (!verbose)
    return;

  for (i = 0; i < max; i++)
    if (rr[i].type == LSART_STUB)
      cli_msg(-1016, "\t\tstubnet %I/%d metric %u", ipa_from_u32(rr[i].id),
	      ipa_mklen(ipa_from_u32(rr[i].data)), rr[i].metric);
#endif
}
