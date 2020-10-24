bfd_open_tx_sk(struct bfd_proto *p, ip_addr local, struct iface *ifa)
{
  sock *sk = sk_new(p->tpool);
  sk->type = SK_UDP;
  sk->saddr = local;
  sk->dport = ifa ? BFD_CONTROL_PORT : BFD_MULTI_CTL_PORT;
  sk->iface = ifa;
  sk->data = p;

  sk->tbsize = BFD_MAX_LEN;
  sk->err_hook = bfd_err_hook;

  /* TODO: configurable ToS, priority and TTL security */
  sk->tos = IP_PREC_INTERNET_CONTROL;
  sk->priority = sk_priority_control;
  sk->ttl = ifa ? 255 : -1;
  sk->flags = SKF_THREAD | SKF_BIND;

#ifdef IPV6
  sk->flags |= SKF_V6ONLY;
#endif

  if (sk_open(sk) < 0)
    goto err;

  sk_start(sk);
  return sk;

 err:
  sk_log_error(sk, p->p.name);
  rfree(sk);
  return NULL;
}
