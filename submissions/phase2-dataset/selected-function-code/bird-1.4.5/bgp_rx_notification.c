static void
bgp_rx_notification(struct bgp_conn *conn, byte *pkt, int len)
{
  struct bgp_proto *p = conn->bgp;
  if (len < 21)
    {
      bgp_error(conn, 1, 2, pkt+16, 2);
      return;
    }

  unsigned code = pkt[19];
  unsigned subcode = pkt[20];
  int err = (code != 6);

  bgp_log_error(p, BE_BGP_RX, "Received", code, subcode, pkt+21, len-21);
  bgp_store_error(p, conn, BE_BGP_RX, (code << 16) | subcode);

#ifndef IPV6
  if ((code == 2) && ((subcode == 4) || (subcode == 7))
      /* Error related to capability:
       * 4 - Peer does not support capabilities at all.
       * 7 - Peer request some capability. Strange unless it is IPv6 only peer.
       */
      && (p->cf->capabilities == 2)
      /* Capabilities are not explicitly enabled or disabled, therefore heuristic is used */
      && (conn->start_state == BSS_CONNECT)
      /* Failed connection attempt have used capabilities */
      && (p->cf->remote_as <= 0xFFFF))
      /* Not possible with disabled capabilities */
    {
      /* We try connect without capabilities */
      log(L_WARN "%s: Capability related error received, retry with capabilities disabled", p->p.name);
      p->start_state = BSS_CONNECT_NOCAP;
      err = 0;
    }
#endif

  bgp_conn_enter_close_state(conn);
  bgp_schedule_packet(conn, PKT_SCHEDULE_CLOSE);

  if (err) 
    {
      bgp_update_startup_delay(p);
      bgp_stop(p, 0);
    }
}
