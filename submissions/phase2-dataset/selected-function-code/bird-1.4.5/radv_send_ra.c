void
radv_send_ra(struct radv_iface *ifa, int shutdown)
{
  struct proto_radv *ra = ifa->ra;

  /* We store prepared RA in tbuf */
  if (!ifa->plen)
    radv_prepare_ra(ifa);

  if (shutdown)
  {
    /*
     * Modify router lifetime to 0, it is not restored because we suppose that
     * the iface will be removed. The preference value also has to be zeroed.
     * (RFC 4191 2.2: If router lifetime is 0, the preference value must be 0.)
     */

    struct radv_ra_packet *pkt = (void *) ifa->sk->tbuf;
    pkt->router_lifetime = 0;
    pkt->flags &= ~RA_PREF_MASK;
  }

  RADV_TRACE(D_PACKETS, "Sending RA via %s", ifa->iface->name);
  sk_send_to(ifa->sk, ifa->plen, AllNodes, 0);
}
