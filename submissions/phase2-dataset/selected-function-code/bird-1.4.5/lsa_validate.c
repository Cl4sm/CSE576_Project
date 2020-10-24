int
lsa_validate(struct ospf_lsa_header *lsa, void *body)
{
  switch (lsa->type)
    {
    case LSA_T_RT:
      return lsa_validate_rt(lsa, body);
    case LSA_T_NET:
      return lsa_validate_net(lsa, body);
    case LSA_T_SUM_NET:
      return lsa_validate_sum_net(lsa, body);
    case LSA_T_SUM_RT:
      return lsa_validate_sum_rt(lsa, body);
    case LSA_T_EXT:
    case LSA_T_NSSA:
      return lsa_validate_ext(lsa, body);
#ifdef OSPFv3
    case LSA_T_LINK:
      return lsa_validate_link(lsa, body);
    case LSA_T_PREFIX:
      return lsa_validate_prefix(lsa, body);
#endif
    default:
      /* In OSPFv3, unknown LSAs are OK,
	 In OSPFv2, unknown LSAs are already rejected
      */
      return 1;
    }
}
