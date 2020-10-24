bgp_check_config(struct bgp_config *c)
{
  int internal = (c->local_as == c->remote_as);

  /* Do not check templates at all */
  if (c->c.class == SYM_TEMPLATE)
    return;


  /* EBGP direct by default, IBGP multihop by default */
  if (c->multihop < 0)
    c->multihop = internal ? 64 : 0;

  /* Different default for gw_mode */
  if (!c->gw_mode)
    c->gw_mode = c->multihop ? GW_RECURSIVE : GW_DIRECT;

  /* Different default based on rs_client */
  if (!c->missing_lladdr)
    c->missing_lladdr = c->rs_client ? MLL_IGNORE : MLL_SELF;

  /* Disable after error incompatible with restart limit action */
  if (c->c.in_limit && (c->c.in_limit->action == PLA_RESTART) && c->disable_after_error)
    c->c.in_limit->action = PLA_DISABLE;


  if (!c->local_as)
    cf_error("Local AS number must be set");

  if (!c->remote_as)
    cf_error("Neighbor must be configured");

  if (!(c->capabilities && c->enable_as4) && (c->remote_as > 0xFFFF))
    cf_error("Neighbor AS number out of range (AS4 not available)");

  if (!internal && c->rr_client)
    cf_error("Only internal neighbor can be RR client");

  if (internal && c->rs_client)
    cf_error("Only external neighbor can be RS client");

  if (c->multihop && (c->gw_mode == GW_DIRECT))
    cf_error("Multihop BGP cannot use direct gateway mode");

  if (c->multihop && (ipa_has_link_scope(c->remote_ip) || 
		      ipa_has_link_scope(c->source_addr)))
    cf_error("Multihop BGP cannot be used with link-local addresses");

  if (c->multihop && c->bfd && ipa_zero(c->source_addr))
    cf_error("Multihop BGP with BFD requires specified source address");

  if ((c->gw_mode == GW_RECURSIVE) && c->c.table->sorted)
    cf_error("BGP in recursive mode prohibits sorted table");

  if (c->deterministic_med && c->c.table->sorted)
    cf_error("BGP with deterministic MED prohibits sorted table");

  if (c->secondary && !c->c.table->sorted)
    cf_error("BGP with secondary option requires sorted table");
}
