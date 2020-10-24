int
radv_sk_open(struct radv_iface *ifa)
{
  sock *sk = sk_new(ifa->ra->p.pool);
  sk->type = SK_IP;
  sk->dport = ICMPV6_PROTO;
  sk->saddr = ifa->addr->ip;

  sk->ttl = 255; /* Mandatory for Neighbor Discovery packets */
  sk->rx_hook = radv_rx_hook;
  sk->tx_hook = radv_tx_hook;
  sk->err_hook = radv_err_hook;
  sk->iface = ifa->iface;
  sk->rbsize = 1024; // bufsize(ifa);
  sk->tbsize = 1024; // bufsize(ifa);
  sk->data = ifa;
  sk->flags = SKF_LADDR_RX;

  if (sk_open(sk) < 0)
    goto err;

  /* We want listen just to ICMPv6 messages of type RS and RA */
  if (sk_set_icmp6_filter(sk, ICMPV6_RS, ICMPV6_RA) < 0)
    goto err;

  if (sk_setup_multicast(sk) < 0)
    goto err;

  if (sk_join_group(sk, AllRouters) < 0)
    goto err;

  ifa->sk = sk;
  return 1;

 err:
  sk_log_error(sk, ifa->ra->p.name);
  rfree(sk);
  return 0;
}
