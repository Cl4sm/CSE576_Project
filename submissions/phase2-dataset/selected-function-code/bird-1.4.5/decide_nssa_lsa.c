decide_nssa_lsa(ort *nf, u32 *rt_metric, ip_addr *rt_fwaddr, u32 *rt_tag)
{
  struct ospf_area *oa = nf->n.oa;
  struct top_hash_entry *en = nf->n.en;
  int propagate;

  if (!rt_is_nssa(nf) || !oa->translate)
    return 0;

  /* Condensed area network found */ 
  if (fib_route(&oa->enet_fib, nf->fn.prefix, nf->fn.pxlen))
    return 0;

  if (!en || (en->lsa.type != LSA_T_NSSA))
    return 0;

  /* We do not store needed data in struct orta, we have to parse the LSA */
  struct ospf_lsa_ext *le = en->lsa_body;

#ifdef OSPFv2
  *rt_fwaddr = le->fwaddr;
  *rt_tag = le->tag;
  propagate = en->lsa.options & OPT_P;
#else /* OSPFv3 */
  u32 *buf = le->rest;
  u8 pxlen = (*buf >> 24);
  u8 pxopts = (*buf >> 16);
  buf += IPV6_PREFIX_WORDS(pxlen);  /* Skip the IP prefix */

  if (pxopts & OPT_PX_NU)
    return 0;

  if (le->metric & LSA_EXT_FBIT)
    buf = lsa_get_ipv6_addr(buf, rt_fwaddr);
  else
    *rt_fwaddr = IPA_NONE;

  if (le->metric & LSA_EXT_TBIT)
    *rt_tag = *buf++;
  else
    *rt_tag = 0;

  propagate = pxopts & OPT_PX_P;
#endif

  if (!propagate || ipa_zero(*rt_fwaddr))
    return 0;

  *rt_metric = le->metric & (METRIC_MASK | LSA_EXT_EBIT);
  return 1;
}
