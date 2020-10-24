static struct bfd_session *
bfd_add_session(struct bfd_proto *p, ip_addr addr, ip_addr local, struct iface *iface)
{
  birdloop_enter(p->loop);

  struct bfd_iface *ifa = bfd_get_iface(p, local, iface);

  struct bfd_session *s = sl_alloc(p->session_slab);
  bzero(s, sizeof(struct bfd_session));

  s->addr = addr;
  s->ifa = ifa;
  s->loc_id = bfd_get_free_id(p);

  HASH_INSERT(p->session_hash_id, HASH_ID, s);
  HASH_INSERT(p->session_hash_ip, HASH_IP, s);


  /* Initialization of state variables - see RFC 5880 6.8.1 */
  s->loc_state = BFD_STATE_DOWN;
  s->rem_state = BFD_STATE_DOWN;
  s->des_min_tx_int = s->des_min_tx_new = ifa->cf->idle_tx_int;
  s->req_min_rx_int = s->req_min_rx_new = ifa->cf->min_rx_int;
  s->rem_min_rx_int = 1;
  s->detect_mult = ifa->cf->multiplier;
  s->passive = ifa->cf->passive;

  s->tx_timer = tm2_new_init(p->tpool, bfd_tx_timer_hook, s, 0, 0);
  s->hold_timer = tm2_new_init(p->tpool, bfd_hold_timer_hook, s, 0, 0);
  bfd_session_update_tx_interval(s);
  bfd_session_control_tx_timer(s, 1);

  init_list(&s->request_list);
  s->last_state_change = now;

  TRACE(D_EVENTS, "Session to %I added", s->addr);

  birdloop_leave(p->loop);

  return s;
}
