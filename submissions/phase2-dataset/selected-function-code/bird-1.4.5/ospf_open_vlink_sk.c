ospf_open_vlink_sk(struct proto_ospf *po)
{
  sock *sk = sk_new(po->proto.pool);
  sk->type = SK_IP;
  sk->dport = OSPF_PROTO;

  /* FIXME: configurable tos/priority ? */
  sk->tos = IP_PREC_INTERNET_CONTROL;
  sk->priority = sk_priority_control;
  sk->err_hook = ospf_verr_hook;

  sk->rbsize = 0;
  sk->tbsize = OSPF_VLINK_MTU;
  sk->data = (void *) po;
  sk->flags = 0;

  if (sk_open(sk) < 0)
    goto err;

#ifdef OSPFv3
  /* 12 is an offset of the checksum in an OSPF packet */
  if (sk_set_ipv6_checksum(sk, 12) < 0)
    goto err;
#endif

  po->vlink_sk = sk;
  return;

 err:
  sk_log_error(sk, po->proto.name);
  log(L_ERR "%s: Cannot open virtual link socket", po->proto.name);
  rfree(sk);
}
