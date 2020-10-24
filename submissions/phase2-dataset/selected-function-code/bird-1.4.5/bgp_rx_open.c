bgp_rx_open(struct bgp_conn *conn, byte *pkt, int len)
{
  struct bgp_conn *other;
  struct bgp_proto *p = conn->bgp;
  unsigned hold;
  u16 base_as;
  u32 id;

  /* Check state */
  if (conn->state != BS_OPENSENT)
    { bgp_error(conn, 5, fsm_err_subcode[conn->state], NULL, 0); return; }

  /* Check message contents */
  if (len < 29 || len != 29 + pkt[28])
    { bgp_error(conn, 1, 2, pkt+16, 2); return; }
  if (pkt[19] != BGP_VERSION)
    { bgp_error(conn, 2, 1, pkt+19, 1); return; } /* RFC 1771 says 16 bits, draft-09 tells to use 8 */
  conn->advertised_as = base_as = get_u16(pkt+20);
  hold = get_u16(pkt+22);
  id = get_u32(pkt+24);
  BGP_TRACE(D_PACKETS, "Got OPEN(as=%d,hold=%d,id=%08x)", conn->advertised_as, hold, id);

  if (bgp_parse_options(conn, pkt+29, pkt[28]))
    return;

  if (hold > 0 && hold < 3)
    { bgp_error(conn, 2, 6, pkt+22, 2); return; }

  if (!id || id == 0xffffffff || id == p->local_id)
    { bgp_error(conn, 2, 3, pkt+24, -4); return; }

  if ((conn->advertised_as != base_as) && (base_as != AS_TRANS))
    log(L_WARN "%s: Peer advertised inconsistent AS numbers", p->p.name);

  if (conn->advertised_as != p->remote_as)
    {
      if (conn->peer_as4_support)
	{
	  u32 val = htonl(conn->advertised_as);
	  bgp_error(conn, 2, 2, (byte *) &val, 4);
	}
      else
	bgp_error(conn, 2, 2, pkt+20, 2);

      return;
    }

  /* Check the other connection */
  other = (conn == &p->outgoing_conn) ? &p->incoming_conn : &p->outgoing_conn;
  switch (other->state)
    {
    case BS_CONNECT:
    case BS_ACTIVE:
      /* Stop outgoing connection attempts */
      bgp_conn_enter_idle_state(other);
      break;

    case BS_IDLE:
    case BS_OPENSENT:
    case BS_CLOSE:
      break;

    case BS_OPENCONFIRM:
      if ((p->local_id < id) == (conn == &p->incoming_conn))
	{
	  /* Should close the other connection */
	  BGP_TRACE(D_EVENTS, "Connection collision, giving up the other connection");
	  bgp_error(other, 6, 7, NULL, 0);
	  break;
	}
      /* Fall thru */
    case BS_ESTABLISHED:
      /* Should close this connection */
      BGP_TRACE(D_EVENTS, "Connection collision, giving up this connection");
      bgp_error(conn, 6, 7, NULL, 0);
      return;
    default:
      bug("bgp_rx_open: Unknown state");
    }

  /* Update our local variables */
  conn->hold_time = MIN(hold, p->cf->hold_time);
  conn->keepalive_time = p->cf->keepalive_time ? : conn->hold_time / 3;
  p->remote_id = id;
  p->as4_session = p->cf->enable_as4 && conn->peer_as4_support;
  p->add_path_rx = (p->cf->add_path & ADD_PATH_RX) && (conn->peer_add_path & ADD_PATH_TX);
  p->add_path_tx = (p->cf->add_path & ADD_PATH_TX) && (conn->peer_add_path & ADD_PATH_RX);
  p->gr_ready = p->cf->gr_mode && conn->peer_gr_able;

  if (p->add_path_tx)
    p->p.accept_ra_types = RA_ANY;

  DBG("BGP: Hold timer set to %d, keepalive to %d, AS to %d, ID to %x, AS4 session to %d\n", conn->hold_time, conn->keepalive_time, p->remote_as, p->remote_id, p->as4_session);

  bgp_schedule_packet(conn, PKT_KEEPALIVE);
  bgp_start_timer(conn->hold_timer, conn->hold_time);
  bgp_conn_enter_openconfirm_state(conn);
}
