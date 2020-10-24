static void
bgp_graceful_close_conn(struct bgp_conn *conn, unsigned subcode)
{
  switch (conn->state)
    {
    case BS_IDLE:
    case BS_CLOSE:
      return;
    case BS_CONNECT:
    case BS_ACTIVE:
      bgp_conn_enter_idle_state(conn);
      return;
    case BS_OPENSENT:
    case BS_OPENCONFIRM:
    case BS_ESTABLISHED:
      bgp_error(conn, 6, subcode, NULL, 0);
      return;
    default:
      bug("bgp_graceful_close_conn: Unknown state %d", conn->state);
    }
}
