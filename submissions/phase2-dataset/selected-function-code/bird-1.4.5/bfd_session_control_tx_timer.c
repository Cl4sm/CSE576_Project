static void
bfd_session_control_tx_timer(struct bfd_session *s, int reset)
{
  // if (!s->opened) goto stop;

  if (s->passive && (s->rem_id == 0))
    goto stop;

  if (s->rem_demand_mode && 
      !s->poll_active && 
      (s->loc_state == BFD_STATE_UP) &&
      (s->rem_state == BFD_STATE_UP))
    goto stop;

  if (s->rem_min_rx_int == 0)
    goto stop;

  /* So TX timer should run */
  if (reset || !tm2_active(s->tx_timer))
  {
    s->last_tx = 0;
    tm2_start(s->tx_timer, 0);
  }

  return;

 stop:
  tm2_stop(s->tx_timer);
  s->last_tx = 0;
}
