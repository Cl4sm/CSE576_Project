static void
bgp_hold_timeout(timer *t)
{
  struct bgp_conn *conn = t->data;
  struct bgp_proto *p = conn->bgp;

  DBG("BGP: Hold timeout\n");

  /* We are already closing the connection - just do hangup */
  if (conn->state == BS_CLOSE)
  {
    BGP_TRACE(D_EVENTS, "Connection stalled");
    bgp_conn_enter_idle_state(conn);
    return;
  }

  /* If there is something in input queue, we are probably congested
     and perhaps just not processed BGP packets in time. */

  if (sk_rx_ready(conn->sk) > 0)
    bgp_start_timer(conn->hold_timer, 10);
  else
    bgp_error(conn, 4, 0, NULL, 0);
}
