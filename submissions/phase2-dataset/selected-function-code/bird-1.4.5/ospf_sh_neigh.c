void
ospf_sh_neigh(struct proto *p, char *iff)
{
  struct ospf_iface *ifa = NULL;
  struct ospf_neighbor *n;
  struct proto_ospf *po = (struct proto_ospf *) p;

  if (p->proto_state != PS_UP)
  {
    cli_msg(-1013, "%s: is not up", p->name);
    cli_msg(0, "");
    return;
  }

  cli_msg(-1013, "%s:", p->name);
  cli_msg(-1013, "%-12s\t%3s\t%-15s\t%-5s\t%-10s %-12s", "Router ID", "Pri",
	  "     State", "DTime", "Interface", "Router IP");
  WALK_LIST(ifa, po->iface_list)
    if ((iff == NULL) || patmatch(iff, ifa->ifname))
      WALK_LIST(n, ifa->neigh_list)
	ospf_sh_neigh_info(n);
  cli_msg(0, "");
}
