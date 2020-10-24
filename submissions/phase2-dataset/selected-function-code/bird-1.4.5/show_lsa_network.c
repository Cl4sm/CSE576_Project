show_lsa_network(struct top_hash_entry *he)
{
  struct ospf_lsa_header *lsa = &(he->lsa);
  struct ospf_lsa_net *ln = he->lsa_body;
  u32 i;

#ifdef OSPFv2
  cli_msg(-1016, "");
  cli_msg(-1016, "\tnetwork %I/%d", ipa_and(ipa_from_u32(lsa->id), ln->netmask), ipa_mklen(ln->netmask));
  cli_msg(-1016, "\t\tdr %R", lsa->rt);
#else /* OSPFv3 */
  cli_msg(-1016, "");
  cli_msg(-1016, "\tnetwork [%R-%u]", lsa->rt, lsa->id);
#endif

  show_lsa_distance(he);

  for (i = 0; i < lsa_net_count(lsa); i++)
    cli_msg(-1016, "\t\trouter %R", ln->routers[i]);
}
