bgp_setup_listen_sk(ip_addr addr, unsigned port, u32 flags)
{
  sock *s = sk_new(&root_pool);
  DBG("BGP: Creating listening socket\n");
  s->type = SK_TCP_PASSIVE;
  s->ttl = 255;
  s->saddr = addr;
  s->sport = port ? port : BGP_PORT;
  s->flags = flags ? 0 : SKF_V6ONLY;
  s->tos = IP_PREC_INTERNET_CONTROL;
  s->rbsize = BGP_RX_BUFFER_SIZE;
  s->tbsize = BGP_TX_BUFFER_SIZE;
  s->rx_hook = bgp_incoming_connection;
  s->err_hook = bgp_listen_sock_err;

  if (sk_open(s) < 0)
    goto err;

  return s;

 err:
  sk_log_error(s, "BGP");
  log(L_ERR "BGP: Cannot open listening socket");
  rfree(s);
  return NULL;
}
