static void
krt_read_addr(struct ks_msg *msg, int scan)
{
  struct ifa_msghdr *ifam = (struct ifa_msghdr *)&msg->rtm;
  void *body = (void *)(ifam + 1);
  sockaddr addr, mask, brd;
  struct iface *iface = NULL;
  struct ifa ifa;
  struct sockaddr null;
  ip_addr iaddr, imask, ibrd;
  int addrs = ifam->ifam_addrs;
  int scope, masklen = -1;
  int new = (ifam->ifam_type == RTM_NEWADDR);

  /* Strange messages with zero (invalid) ifindex appear on OpenBSD */
  if (ifam->ifam_index == 0)
    return;

  if(!(iface = if_find_by_index(ifam->ifam_index)))
  {
    log(L_ERR "KIF: Received address message for unknown interface %d", ifam->ifam_index);
    return;
  }

  GETADDR (&null, RTA_DST);
  GETADDR (&null, RTA_GATEWAY);
  GETADDR (&mask, RTA_NETMASK);
  GETADDR (&null, RTA_GENMASK);
  GETADDR (&null, RTA_IFP);
  GETADDR (&addr, RTA_IFA);
  GETADDR (&null, RTA_AUTHOR);
  GETADDR (&brd, RTA_BRD);

  /* Some other family address */
  if (addr.sa.sa_family != BIRD_AF)
    return;

  iaddr = ipa_from_sa(&addr);
  imask = ipa_from_sa(&mask);
  ibrd  = ipa_from_sa(&brd);


  if ((masklen = ipa_mklen(imask)) < 0)
  {
    log(L_ERR "KIF: Invalid masklen %I for %s", imask, iface->name);
    return;
  }

#ifdef IPV6
  /* Clean up embedded interface ID returned in link-local address */

  if (ipa_has_link_scope(iaddr))
    _I0(iaddr) = 0xfe800000;

  if (ipa_has_link_scope(ibrd))
    _I0(ibrd) = 0xfe800000;
#endif


  bzero(&ifa, sizeof(ifa));
  ifa.iface = iface;
  ifa.ip = iaddr;
  ifa.pxlen = masklen;

  scope = ipa_classify(ifa.ip);
  if (scope < 0)
  {
    log(L_ERR "KIF: Invalid interface address %I for %s", ifa.ip, iface->name);
    return;
  }
  ifa.scope = scope & IADDR_SCOPE_MASK;

  if (masklen < BITS_PER_IP_ADDRESS)
  {
    ifa.prefix = ipa_and(ifa.ip, ipa_mkmask(masklen));

    if (masklen == (BITS_PER_IP_ADDRESS - 1))
      ifa.opposite = ipa_opposite_m1(ifa.ip);

#ifndef IPV6
    if (masklen == (BITS_PER_IP_ADDRESS - 2))
      ifa.opposite = ipa_opposite_m2(ifa.ip);
#endif

    if (iface->flags & IF_BROADCAST)
      ifa.brd = ibrd;

    if (!(iface->flags & IF_MULTIACCESS))
      ifa.opposite = ibrd;
  }
  else if (!(iface->flags & IF_MULTIACCESS) && ipa_nonzero(ibrd))
  {
    ifa.prefix = ifa.opposite = ibrd;
    ifa.flags |= IA_PEER;
  }
  else
  {
    ifa.prefix = ifa.ip;
    ifa.flags |= IA_HOST;
  }

  if (new)
    ifa_update(&ifa);
  else
    ifa_delete(&ifa);

  if (!scan)
    if_end_partial_update(iface);
}
