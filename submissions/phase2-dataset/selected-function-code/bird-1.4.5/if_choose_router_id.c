u32
if_choose_router_id(struct iface_patt *mask, u32 old_id)
{
#ifndef IPV6
  struct iface *i;
  struct ifa *a, *b;

  b = NULL;
  WALK_LIST(i, iface_list)
    {
      if (!(i->flags & IF_ADMIN_UP) ||
	  (i->flags & IF_SHUTDOWN))
	continue;

      WALK_LIST(a, i->addrs)
	{
	  if (a->flags & IA_SECONDARY)
	    continue;

	  if (a->scope <= SCOPE_LINK)
	    continue;

	  /* Check pattern if specified */
	  if (mask && !iface_patt_match(mask, i, a))
	    continue;

	  /* No pattern or pattern matched */
	  if (!b || ipa_to_u32(a->ip) < ipa_to_u32(b->ip))
	    b = a;
	}
    }

  if (!b)
    return 0;

  u32 id = ipa_to_u32(b->ip);
  if (id != old_id)
    log(L_INFO "Chosen router ID %R according to interface %s", id, b->iface->name);

  return id;

#else
  return 0;
#endif
}
