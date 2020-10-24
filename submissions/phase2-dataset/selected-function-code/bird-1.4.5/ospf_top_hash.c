static inline unsigned
ospf_top_hash(struct top_graph *f, u32 domain, u32 lsaid, u32 rtrid, u32 type)
{
  /* In OSPFv2, we don't know Router ID when looking for network LSAs.
     In OSPFv3, we don't know LSA ID when looking for router LSAs.
     In both cases, there is (usually) just one (or small number)
     appropriate LSA, so we just clear unknown part of key. */

  return (
#ifdef OSPFv2
	  ((type == LSA_T_NET) ? 0 : ospf_top_hash_u32(rtrid)) +
	  ospf_top_hash_u32(lsaid) + 
#else /* OSPFv3 */
	  ospf_top_hash_u32(rtrid) +
	  ((type == LSA_T_RT) ? 0 : ospf_top_hash_u32(lsaid)) +
#endif
	  type + domain) & f->hash_mask;

  /*
  return (ospf_top_hash_u32(lsaid) + ospf_top_hash_u32(rtrid) +
	  type + areaid) & f->hash_mask;
  */
}
