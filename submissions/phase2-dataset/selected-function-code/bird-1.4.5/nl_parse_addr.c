nl_parse_addr(struct nlmsghdr *h, int scan)
{
  struct ifaddrmsg *i;
  struct rtattr *a[IFA_ANYCAST+1];
  int new = h->nlmsg_type == RTM_NEWADDR;
  struct ifa ifa;
  struct iface *ifi;
  int scope;

  if (!(i = nl_checkin(h, sizeof(*i))) || !nl_parse_attrs(IFA_RTA(i), a, sizeof(a)))
    return;
  if (i->ifa_family != BIRD_AF)
    return;
  if (!a[IFA_ADDRESS] || RTA_PAYLOAD(a[IFA_ADDRESS]) != sizeof(ip_addr)
#ifdef IPV6
      || a[IFA_LOCAL] && RTA_PAYLOAD(a[IFA_LOCAL]) != sizeof(ip_addr)
#else
      || !a[IFA_LOCAL] || RTA_PAYLOAD(a[IFA_LOCAL]) != sizeof(ip_addr)
      || (a[IFA_BROADCAST] && RTA_PAYLOAD(a[IFA_BROADCAST]) != sizeof(ip_addr))
#endif
      )
    {
      log(L_ERR "nl_parse_addr: Malformed message received");
      return;
    }

  ifi = if_find_by_index(i->ifa_index);
  if (!ifi)
    {
      log(L_ERR "KIF: Received address message for unknown interface %d", i->ifa_index);
      return;
    }

  bzero(&ifa, sizeof(ifa));
  ifa.iface = ifi;
  if (i->ifa_flags & IFA_F_SECONDARY)
    ifa.flags |= IA_SECONDARY;

  /* IFA_LOCAL can be unset for IPv6 interfaces */
  memcpy(&ifa.ip, RTA_DATA(a[IFA_LOCAL] ? : a[IFA_ADDRESS]), sizeof(ifa.ip));
  ipa_ntoh(ifa.ip);
  ifa.pxlen = i->ifa_prefixlen;
  if (i->ifa_prefixlen > BITS_PER_IP_ADDRESS)
    {
      log(L_ERR "KIF: Invalid prefix length for interface %s: %d", ifi->name, i->ifa_prefixlen);
      new = 0;
    }
  if (i->ifa_prefixlen == BITS_PER_IP_ADDRESS)
    {
      ip_addr addr;
      memcpy(&addr, RTA_DATA(a[IFA_ADDRESS]), sizeof(addr));
      ipa_ntoh(addr);
      ifa.prefix = ifa.brd = addr;

      /* It is either a host address or a peer address */
      if (ipa_equal(ifa.ip, addr))
	ifa.flags |= IA_HOST;
      else
	{
	  ifa.flags |= IA_PEER;
	  ifa.opposite = addr;
	}
    }
  else
    {
      ip_addr netmask = ipa_mkmask(ifa.pxlen);
      ifa.prefix = ipa_and(ifa.ip, netmask);
      ifa.brd = ipa_or(ifa.ip, ipa_not(netmask));
      if (i->ifa_prefixlen == BITS_PER_IP_ADDRESS - 1)
	ifa.opposite = ipa_opposite_m1(ifa.ip);

#ifndef IPV6
      if (i->ifa_prefixlen == BITS_PER_IP_ADDRESS - 2)
	ifa.opposite = ipa_opposite_m2(ifa.ip);

      if ((ifi->flags & IF_BROADCAST) && a[IFA_BROADCAST])
	{
	  ip_addr xbrd;
	  memcpy(&xbrd, RTA_DATA(a[IFA_BROADCAST]), sizeof(xbrd));
	  ipa_ntoh(xbrd);
	  if (ipa_equal(xbrd, ifa.prefix) || ipa_equal(xbrd, ifa.brd))
	    ifa.brd = xbrd;
	  else if (ifi->flags & IF_TMP_DOWN) /* Complain only during the first scan */
	    log(L_ERR "KIF: Invalid broadcast address %I for %s", xbrd, ifi->name);
	}
#endif
    }

  scope = ipa_classify(ifa.ip);
  if (scope < 0)
    {
      log(L_ERR "KIF: Invalid interface address %I for %s", ifa.ip, ifi->name);
      return;
    }
  ifa.scope = scope & IADDR_SCOPE_MASK;

  DBG("KIF: IF%d(%s): %s IPA %I, flg %x, net %I/%d, brd %I, opp %I\n",
      ifi->index, ifi->name,
      new ? "added" : "removed",
      ifa.ip, ifa.flags, ifa.prefix, ifa.pxlen, ifa.brd, ifa.opposite);

  if (new)
    ifa_update(&ifa);
  else
    ifa_delete(&ifa);

  if (!scan)
    if_end_partial_update(ifi);
}
