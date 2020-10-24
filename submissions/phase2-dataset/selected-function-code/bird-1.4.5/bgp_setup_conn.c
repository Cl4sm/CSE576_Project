bgp_setup_conn(struct bgp_proto *p, struct bgp_conn *conn)
{
  timer *t;

  conn->sk = NULL;
  conn->bgp = p;
  conn->packets_to_send = 0;

  t = conn->connect_retry_timer = tm_new(p->p.pool);
  t->hook = bgp_connect_timeout;
  t->data = conn;
  t = conn->hold_timer = tm_new(p->p.pool);
  t->hook = bgp_hold_timeout;
  t->data = conn;
  t = conn->keepalive_timer = tm_new(p->p.pool);
  t->hook = bgp_keepalive_timeout;
  t->data = conn;
  conn->tx_ev = ev_new(p->p.pool);
  conn->tx_ev->hook = bgp_kick_tx;
  conn->tx_ev->data = conn;
}
