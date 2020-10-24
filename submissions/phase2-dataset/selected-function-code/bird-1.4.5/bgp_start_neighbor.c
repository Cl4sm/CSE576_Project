bgp_start_neighbor(struct bgp_proto *p)
{
  /* Called only for single-hop BGP sessions */

  if (ipa_zero(p->source_addr))
    p->source_addr = p->neigh->ifa->ip; 

#ifdef IPV6
  {
    struct ifa *a;
    p->local_link = IPA_NONE;
    WALK_LIST(a, p->neigh->iface->addrs)
      if (a->scope == SCOPE_LINK)
        {
	  p->local_link = a->ip;
	  break;
	}

    if (! ipa_nonzero(p->local_link))
      log(L_WARN "%s: Missing link local address on interface %s", p->p.name,  p->neigh->iface->name);

    DBG("BGP: Selected link-level address %I\n", p->local_link);
  }
#endif

  bgp_initiate(p);
}
