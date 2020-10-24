bgp_send_open(struct bgp_conn *conn)
{
  conn->start_state = conn->bgp->start_state;

  // Default values, possibly changed by receiving capabilities.
  conn->advertised_as = 0;
  conn->peer_refresh_support = 0;
  conn->peer_as4_support = 0;
  conn->peer_add_path = 0;
  conn->peer_gr_aware = 0;
  conn->peer_gr_able = 0;
  conn->peer_gr_time = 0;
  conn->peer_gr_flags = 0;
  conn->peer_gr_aflags = 0;

  DBG("BGP: Sending open\n");
  conn->sk->rx_hook = bgp_rx;
  conn->sk->tx_hook = bgp_tx;
  tm_stop(conn->connect_retry_timer);
  bgp_schedule_packet(conn, PKT_OPEN);
  bgp_conn_set_state(conn, BS_OPENSENT);
  bgp_start_timer(conn->hold_timer, conn->bgp->cf->initial_hold_time);
}
