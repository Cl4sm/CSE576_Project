configured_stubnet(struct ospf_area *oa, struct ifa *a)
{
  if (!oa->ac)
    return 0;

  /* Does not work for IA_PEER addresses, but it is not called on these */
  struct ospf_stubnet_config *sn;
  WALK_LIST(sn, oa->ac->stubnet_list)
    {
      if (sn->summary)
	{
	  if (ipa_in_net(a->prefix, sn->px.addr, sn->px.len) && (a->pxlen >= sn->px.len))
	    return 1;
	}
      else
	{
	  if (ipa_equal(a->prefix, sn->px.addr) && (a->pxlen == sn->px.len))
	    return 1;
	}
    }
  return 0;
}
