void
if_show(void)
{
  struct iface *i;
  struct ifa *a;
  char *type;

  WALK_LIST(i, iface_list)
    {
      if (i->flags & IF_SHUTDOWN)
	continue;

      cli_msg(-1001, "%s %s (index=%d)", i->name, (i->flags & IF_UP) ? "up" : "DOWN", i->index);
      if (!(i->flags & IF_MULTIACCESS))
	type = "PtP";
      else
	type = "MultiAccess";
      cli_msg(-1004, "\t%s%s%s Admin%s Link%s%s%s MTU=%d",
	      type,
	      (i->flags & IF_BROADCAST) ? " Broadcast" : "",
	      (i->flags & IF_MULTICAST) ? " Multicast" : "",
	      (i->flags & IF_ADMIN_UP) ? "Up" : "Down",
	      (i->flags & IF_LINK_UP) ? "Up" : "Down",
	      (i->flags & IF_LOOPBACK) ? " Loopback" : "",
	      (i->flags & IF_IGNORE) ? " Ignored" : "",
	      i->mtu);
      if (i->addr)
	if_show_addr(i->addr);
      WALK_LIST(a, i->addrs)
	if (a != i->addr)
	  if_show_addr(a);
    }
  cli_msg(0, "");
}
