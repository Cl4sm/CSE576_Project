static inline void
show_lsa_external(struct top_hash_entry *he)
{
  struct ospf_lsa_ext *ext = he->lsa_body;
  char str_via[STD_ADDRESS_P_LENGTH + 8] = "";
  char str_tag[16] = "";
  ip_addr ip, rt_fwaddr;
  int pxlen, ebit, rt_fwaddr_valid;
  u32 rt_tag, rt_metric;

  if (he->lsa.type == LSA_T_EXT)
    he->domain = 0; /* Unmark the LSA */

  rt_metric = ext->metric & METRIC_MASK;
  ebit = ext->metric & LSA_EXT_EBIT;
#ifdef OSPFv2
  ip = ipa_and(ipa_from_u32(he->lsa.id), ext->netmask);
  pxlen = ipa_mklen(ext->netmask);
  rt_fwaddr = ext->fwaddr;
  rt_fwaddr_valid = !ipa_equal(rt_fwaddr, IPA_NONE);
  rt_tag = ext->tag;
#else /* OSPFv3 */
  u8 pxopts;
  u16 rest;
  u32 *buf = ext->rest;
  buf = lsa_get_ipv6_prefix(buf, &ip, &pxlen, &pxopts, &rest);

  rt_fwaddr_valid = ext->metric & LSA_EXT_FBIT;
  if (rt_fwaddr_valid)
    buf = lsa_get_ipv6_addr(buf, &rt_fwaddr);
  else 
    rt_fwaddr = IPA_NONE;

  if (ext->metric & LSA_EXT_TBIT)
    rt_tag = *buf++;
  else
    rt_tag = 0;
#endif
  
  if (rt_fwaddr_valid)
    bsprintf(str_via, " via %I", rt_fwaddr);

  if (rt_tag)
    bsprintf(str_tag, " tag %08x", rt_tag);

  cli_msg(-1016, "\t\t%s %I/%d metric%s %u%s%s",
	  (he->lsa.type == LSA_T_NSSA) ? "nssa-ext" : "external",
	  ip, pxlen, ebit ? "2" : "", rt_metric, str_via, str_tag);
}
