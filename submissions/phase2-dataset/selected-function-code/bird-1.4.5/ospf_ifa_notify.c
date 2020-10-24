ospf_ifa_notify(struct proto *p, unsigned flags, struct ifa *a)
{
  struct proto_ospf *po = (struct proto_ospf *) p;

  if (a->flags & IA_SECONDARY)
    return;

  if (a->scope <= SCOPE_LINK)
    return;

  /* In OSPFv2, we create OSPF iface for each address. */
  if (flags & IF_CHANGE_UP)
  {
    int done = 0;
    struct ospf_area *oa;
    WALK_LIST(oa, po->area_list)
    {
      struct ospf_iface_patt *ip;
      if (ip = ospf_iface_patt_find(oa->ac, a))
      {
	if (!done)
	  ospf_iface_new(oa, a, ip);
	done++;
      }
    }

    if (done > 1)
      log(L_WARN "%s: Interface %s (IP %I) matches for multiple areas", p->name,  a->iface->name, a->ip);
  }

  if (flags & IF_CHANGE_DOWN)
  {
    struct ospf_iface *ifa, *ifx;
    WALK_LIST_DELSAFE(ifa, ifx, po->iface_list)
    {
      if ((ifa->type != OSPF_IT_VLINK) && (ifa->addr == a))
	ospf_iface_remove(ifa);
      /* See a note in ospf_iface_notify() */
    }
  }
}
