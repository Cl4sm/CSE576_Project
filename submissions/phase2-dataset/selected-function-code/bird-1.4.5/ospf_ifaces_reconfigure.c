void
ospf_ifaces_reconfigure(struct ospf_area *oa, struct ospf_area_config *nac)
{
  struct proto *p = &oa->po->proto;
  struct ospf_iface_patt *ip;
  struct iface *iface;
  struct ifa *a;

  WALK_LIST(iface, iface_list)
  {
    if (! (iface->flags & IF_UP))
      continue;

    WALK_LIST(a, iface->addrs)
    {
      if (a->flags & IA_SECONDARY)
	continue;

      if (a->scope <= SCOPE_LINK)
	continue;

      if (ip = ospf_iface_patt_find(oa->ac, a))
      {
	/* Main inner loop */
	struct ospf_iface *ifa = ospf_iface_find_by_key(oa, a);
	if (ifa)
	{
	  if (ospf_iface_reconfigure(ifa, ip))
	    continue;

	  /* Hard restart */
	  log(L_INFO "%s: Restarting interface %s (%I/%d) in area %R",
	      p->name, ifa->ifname, a->prefix, a->pxlen, oa->areaid);
	  ospf_iface_shutdown(ifa);
	  ospf_iface_remove(ifa);
	}
	
	ospf_iface_new(oa, a, ip);
      }
    }
  }
}
