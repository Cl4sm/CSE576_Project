static void
nl_parse_link(struct nlmsghdr *h, int scan)
{
  struct ifinfomsg *i;
  struct rtattr *a[IFLA_WIRELESS+1];
  int new = h->nlmsg_type == RTM_NEWLINK;
  struct iface f = {};
  struct iface *ifi;
  char *name;
  u32 mtu;
  unsigned int fl;

  if (!(i = nl_checkin(h, sizeof(*i))) || !nl_parse_attrs(IFLA_RTA(i), a, sizeof(a)))
    return;
  if (!a[IFLA_IFNAME] || RTA_PAYLOAD(a[IFLA_IFNAME]) < 2 ||
      !a[IFLA_MTU] || RTA_PAYLOAD(a[IFLA_MTU]) != 4)
    {
      if (scan || !a[IFLA_WIRELESS])
        log(L_ERR "nl_parse_link: Malformed message received");
      return;
    }
  name = RTA_DATA(a[IFLA_IFNAME]);
  memcpy(&mtu, RTA_DATA(a[IFLA_MTU]), sizeof(u32));

  ifi = if_find_by_index(i->ifi_index);
  if (!new)
    {
      DBG("KIF: IF%d(%s) goes down\n", i->ifi_index, name);
      if (!ifi)
	return;

      if_delete(ifi);
    }
  else
    {
      DBG("KIF: IF%d(%s) goes up (mtu=%d,flg=%x)\n", i->ifi_index, name, mtu, i->ifi_flags);
      if (ifi && strncmp(ifi->name, name, sizeof(ifi->name)-1))
	if_delete(ifi);

      strncpy(f.name, name, sizeof(f.name)-1);
      f.index = i->ifi_index;
      f.mtu = mtu;

      fl = i->ifi_flags;
      if (fl & IFF_UP)
	f.flags |= IF_ADMIN_UP;
      if (fl & IFF_LOWER_UP)
	f.flags |= IF_LINK_UP;
      if (fl & IFF_LOOPBACK)		/* Loopback */
	f.flags |= IF_MULTIACCESS | IF_LOOPBACK | IF_IGNORE;
      else if (fl & IFF_POINTOPOINT)	/* PtP */
	f.flags |= IF_MULTICAST;
      else if (fl & IFF_BROADCAST)	/* Broadcast */
	f.flags |= IF_MULTIACCESS | IF_BROADCAST | IF_MULTICAST;
      else
	f.flags |= IF_MULTIACCESS;	/* NBMA */

      ifi = if_update(&f);

      if (!scan)
	if_end_partial_update(ifi);
    }
}
