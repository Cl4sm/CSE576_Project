bfd_session_process_ctl(struct bfd_session *s, u8 flags, u32 old_tx_int, u32 old_rx_int)
{
  if (s->poll_active && (flags & BFD_FLAG_FINAL))
    bfd_session_terminate_poll(s);

  if ((s->des_min_tx_int != old_tx_int) || (s->rem_min_rx_int != old_rx_int))
    bfd_session_update_tx_interval(s);

  bfd_session_update_detection_time(s, 1);

  /* Update session state */
  int next_state = 0;
  int diag = BFD_DIAG_NOTHING;

  switch (s->loc_state)
  {
  case BFD_STATE_ADMIN_DOWN:
    return;

  case BFD_STATE_DOWN:
    if (s->rem_state == BFD_STATE_DOWN)		next_state = BFD_STATE_INIT;
    else if (s->rem_state == BFD_STATE_INIT)	next_state = BFD_STATE_UP;
    break;

  case BFD_STATE_INIT:
    if (s->rem_state == BFD_STATE_ADMIN_DOWN)	next_state = BFD_STATE_DOWN, diag = BFD_DIAG_NEIGHBOR_DOWN;
    else if (s->rem_state >= BFD_STATE_INIT)	next_state = BFD_STATE_UP;
    break;

  case BFD_STATE_UP:
    if (s->rem_state <= BFD_STATE_DOWN)		next_state = BFD_STATE_DOWN, diag = BFD_DIAG_NEIGHBOR_DOWN;
    break;
  }

  if (next_state)
    bfd_session_update_state(s, next_state, diag);

  bfd_session_control_tx_timer(s, 0);

  if (flags & BFD_FLAG_POLL)
    bfd_send_ctl(s->ifa->bfd, s, 1);
}
