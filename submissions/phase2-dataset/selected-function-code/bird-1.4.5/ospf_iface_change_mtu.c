static void
ospf_iface_change_mtu(struct proto_ospf *po, struct ospf_iface *ifa)
{
  struct proto *p = &po->proto;

  /* ifa is not vlink */

  OSPF_TRACE(D_EVENTS, "Changing MTU on interface %s", ifa->ifname);

  ifa->tx_length = ifa_tx_length(ifa);

  if (!ifa->sk)
    return;

  /* We do not shrink dynamic buffers */
  uint bsize = ifa_bufsize(ifa);
  if (bsize > ifa->sk->rbsize)
    sk_set_rbsize(ifa->sk, bsize);
  if (bsize > ifa->sk->tbsize)
    sk_set_tbsize(ifa->sk, bsize);
}
