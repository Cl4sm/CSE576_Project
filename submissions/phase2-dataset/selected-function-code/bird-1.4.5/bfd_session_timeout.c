bfd_session_timeout(struct bfd_session *s)
{
  struct bfd_proto *p = s->ifa->bfd;

  TRACE(D_EVENTS, "Session to %I expired", s->addr);

  s->rem_state = BFD_STATE_DOWN;
  s->rem_id = 0;
  s->rem_min_tx_int = 0;
  s->rem_min_rx_int = 1;
  s->rem_demand_mode = 0;
  s->rem_detect_mult = 0;

  s->poll_active = 0;
  s->poll_scheduled = 0;

  bfd_session_update_state(s, BFD_STATE_DOWN, BFD_DIAG_TIMEOUT);

  bfd_session_control_tx_timer(s, 1);
}
