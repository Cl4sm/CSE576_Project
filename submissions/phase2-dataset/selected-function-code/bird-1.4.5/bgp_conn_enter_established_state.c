bgp_conn_enter_established_state(struct bgp_conn *conn)
{
  struct bgp_proto *p = conn->bgp;
 
  BGP_TRACE(D_EVENTS, "BGP session established");
  DBG("BGP: UP!!!\n");

  /* For multi-hop BGP sessions */
  if (ipa_zero(p->source_addr))
    p->source_addr = conn->sk->saddr;

  p->conn = conn;
  p->last_error_class = 0;
  p->last_error_code = 0;
  bgp_init_bucket_table(p);
  bgp_init_prefix_table(p, 8);

  int peer_gr_ready = conn->peer_gr_aware && !(conn->peer_gr_flags & BGP_GRF_RESTART);

  if (p->p.gr_recovery && !peer_gr_ready)
    proto_graceful_restart_unlock(&p->p);

  if (p->p.gr_recovery && (p->cf->gr_mode == BGP_GR_ABLE) && peer_gr_ready)
    p->p.gr_wait = 1;

  if (p->gr_active)
    tm_stop(p->gr_timer);

  if (p->gr_active && (!conn->peer_gr_able || !(conn->peer_gr_aflags & BGP_GRF_FORWARDING)))
    bgp_graceful_restart_done(p);

  bgp_conn_set_state(conn, BS_ESTABLISHED);
  proto_notify_state(&p->p, PS_UP);
}
