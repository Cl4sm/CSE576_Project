global_commit(struct config *new, struct config *old)
{
  if (!old)
    return 0;

  if (!ipa_equal(old->listen_bgp_addr, new->listen_bgp_addr) ||
      (old->listen_bgp_port != new->listen_bgp_port) ||
      (old->listen_bgp_flags != new->listen_bgp_flags))
    log(L_WARN "Reconfiguration of BGP listening socket not implemented, please restart BIRD.");

  if (!new->router_id)
    {
      new->router_id = old->router_id;

      if (new->router_id_from)
	{
	  u32 id = if_choose_router_id(new->router_id_from, old->router_id);
	  if (!id)
	    log(L_WARN "Cannot determine router ID, using old one");
	  else
	    new->router_id = id;
	}
    }

  return 0;
}
