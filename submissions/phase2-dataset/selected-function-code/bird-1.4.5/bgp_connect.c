bgp_connect(struct bgp_proto *p)	/* Enter Connect state and start establishing connection */
{
  sock *s;
  struct bgp_conn *conn = &p->outgoing_conn;
  int hops = p->cf->multihop ? : 1;

  DBG("BGP: Connecting\n");
  s = sk_new(p->p.pool);
  s->type = SK_TCP_ACTIVE;
  s->saddr = p->source_addr;
  s->daddr = p->cf->remote_ip;
  s->dport = p->cf->remote_port;
  s->iface = p->neigh ? p->neigh->iface : NULL;
  s->ttl = p->cf->ttl_security ? 255 : hops;
  s->rbsize = BGP_RX_BUFFER_SIZE;
  s->tbsize = BGP_TX_BUFFER_SIZE;
  s->tos = IP_PREC_INTERNET_CONTROL;
  s->password = p->cf->password;
  s->tx_hook = bgp_connected;
  BGP_TRACE(D_EVENTS, "Connecting to %I%J from local address %I%J", s->daddr, p->cf->iface,
	    s->saddr, ipa_has_link_scope(s->saddr) ? s->iface : NULL);
  bgp_setup_conn(p, conn);
  bgp_setup_sk(conn, s);
  bgp_conn_set_state(conn, BS_CONNECT);

  if (sk_open(s) < 0)
    goto err;

  /* Set minimal receive TTL if needed */
  if (p->cf->ttl_security)
    if (sk_set_min_ttl(s, 256 - hops) < 0)
      goto err;

  DBG("BGP: Waiting for connect success\n");
  bgp_start_timer(conn->connect_retry_timer, p->cf->connect_retry_time);
  return;

 err:
  sk_log_error(s, p->p.name);
  bgp_sock_err(s, 0);
  return;
}
