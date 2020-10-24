ospf_sk_open(struct ospf_iface *ifa)
{
  struct proto_ospf *po = ifa->oa->po;

  sock *sk = sk_new(ifa->pool);
  sk->type = SK_IP;
  sk->dport = OSPF_PROTO;
  sk->saddr = ifa->addr->ip;
  sk->iface = ifa->iface;

  sk->tos = ifa->cf->tx_tos;
  sk->priority = ifa->cf->tx_priority;
  sk->rx_hook = ospf_rx_hook;
  // sk->tx_hook = ospf_tx_hook;
  sk->err_hook = ospf_err_hook;
  sk->rbsize = sk->tbsize = ifa_bufsize(ifa);
  sk->data = (void *) ifa;
  sk->flags = SKF_LADDR_RX | (ifa->check_ttl ? SKF_TTL_RX : 0);
  sk->ttl = ifa->cf->ttl_security ? 255 : 1;

  if (sk_open(sk) < 0)
    goto err;

#ifdef OSPFv3
  /* 12 is an offset of the checksum in an OSPF packet */
  if (sk_set_ipv6_checksum(sk, 12) < 0)
    goto err;
#endif

  if ((ifa->type == OSPF_IT_BCAST) || (ifa->type == OSPF_IT_PTP))
  {
    if (ifa->cf->real_bcast)
    {
      ifa->all_routers = ifa->addr->brd;

      if (sk_setup_broadcast(sk) < 0)
        goto err;
    }
    else
    {
      ifa->all_routers = AllSPFRouters;

      if (sk_setup_multicast(sk) < 0)
        goto err;

      if (sk_join_group(sk, ifa->all_routers) < 0)
        goto err;
    }
  }

  ifa->sk = sk;
  ifa->sk_dr = 0;
  return 1;

 err:
  sk_log_error(sk, po->proto.name);
  rfree(sk);
  return 0;
}
