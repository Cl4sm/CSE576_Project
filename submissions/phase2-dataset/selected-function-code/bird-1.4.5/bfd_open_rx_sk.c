bfd_open_rx_sk(struct bfd_proto *p, int multihop)
{
  sock *sk = sk_new(p->tpool);
  sk->type = SK_UDP;
  sk->sport = !multihop ? BFD_CONTROL_PORT : BFD_MULTI_CTL_PORT;
  sk->data = p;

  sk->rbsize = BFD_MAX_LEN;
  sk->rx_hook = bfd_rx_hook;
  sk->err_hook = bfd_err_hook;

  /* TODO: configurable ToS and priority */
  sk->tos = IP_PREC_INTERNET_CONTROL;
  sk->priority = sk_priority_control;
  sk->flags = SKF_THREAD | SKF_LADDR_RX | (!multihop ? SKF_TTL_RX : 0);

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
