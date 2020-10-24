static int
radv_rx_hook(sock *sk, int size)
{
  struct radv_iface *ifa = sk->data;
  struct proto_radv *ra = ifa->ra;

  /* We want just packets from sk->iface */
  if (sk->lifindex != sk->iface->index)
    return 1;

  if (ipa_equal(sk->faddr, ifa->addr->ip))
    return 1;

  if (size < 8)
    return 1;

  byte *buf = sk->rbuf;

  if (buf[1] != 0)
    return 1;

  /* Validation is a bit sloppy - Hop Limit is not checked and
     length of options is ignored for RS and left to later for RA */

  switch (buf[0])
  {
  case ICMPV6_RS:
    RADV_TRACE(D_PACKETS, "Received RS from %I via %s",
	       sk->faddr, ifa->iface->name);
    radv_iface_notify(ifa, RA_EV_RS);
    return 1;

  case ICMPV6_RA:
    RADV_TRACE(D_PACKETS, "Received RA from %I via %s",
	       sk->faddr, ifa->iface->name);
    /* FIXME - there should be some checking of received RAs, but we just ignore them */
    return 1;

  default:
    return 1;
  }
}
