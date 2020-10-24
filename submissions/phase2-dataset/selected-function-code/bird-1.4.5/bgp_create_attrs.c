static int
bgp_create_attrs(struct bgp_proto *p, rte *e, ea_list **attrs, struct linpool *pool)
{
  ea_list *ea = lp_alloc(pool, sizeof(ea_list) + 4*sizeof(eattr));
  rta *rta = e->attrs;
  byte *z;

  ea->next = *attrs;
  *attrs = ea;
  ea->flags = EALF_SORTED;
  ea->count = 4;

  bgp_set_attr(ea->attrs, BA_ORIGIN,
       ((rta->source == RTS_OSPF_EXT1) || (rta->source == RTS_OSPF_EXT2)) ? ORIGIN_INCOMPLETE : ORIGIN_IGP);

  if (p->is_internal)
    bgp_set_attr_wa(ea->attrs+1, pool, BA_AS_PATH, 0);
  else
    {
      z = bgp_set_attr_wa(ea->attrs+1, pool, BA_AS_PATH, 6);
      z[0] = AS_PATH_SEQUENCE;
      z[1] = 1;				/* 1 AS */
      put_u32(z+2, p->local_as);
    }

  /* iBGP -> use gw, eBGP multi-hop -> use source_addr,
     eBGP single-hop -> use gw if on the same iface */
  z = bgp_set_attr_wa(ea->attrs+2, pool, BA_NEXT_HOP, NEXT_HOP_LENGTH);
  if (p->cf->next_hop_self ||
      rta->dest != RTD_ROUTER ||
      ipa_equal(rta->gw, IPA_NONE) ||
      ipa_has_link_scope(rta->gw) ||
      (!p->is_internal && !p->cf->next_hop_keep &&
       (!p->neigh || (rta->iface != p->neigh->iface))))
    set_next_hop(z, p->source_addr);
  else
    set_next_hop(z, rta->gw);

  bgp_set_attr(ea->attrs+3, BA_LOCAL_PREF, p->cf->default_local_pref);

  return 0;				/* Leave decision to the filters */
}
