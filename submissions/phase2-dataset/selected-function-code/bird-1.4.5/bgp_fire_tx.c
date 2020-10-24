bgp_fire_tx(struct bgp_conn *conn)
{
  struct bgp_proto *p = conn->bgp;
  unsigned int s = conn->packets_to_send;
  sock *sk = conn->sk;
  byte *buf, *pkt, *end;
  int type;

  if (!sk)
    {
      conn->packets_to_send = 0;
      return 0;
    }
  buf = sk->tbuf;
  pkt = buf + BGP_HEADER_LENGTH;

  if (s & (1 << PKT_SCHEDULE_CLOSE))
    {
      /* We can finally close connection and enter idle state */
      bgp_conn_enter_idle_state(conn);
      return 0;
    }
  if (s & (1 << PKT_NOTIFICATION))
    {
      s = 1 << PKT_SCHEDULE_CLOSE;
      type = PKT_NOTIFICATION;
      end = bgp_create_notification(conn, pkt);
    }
  else if (s & (1 << PKT_KEEPALIVE))
    {
      s &= ~(1 << PKT_KEEPALIVE);
      type = PKT_KEEPALIVE;
      end = pkt;			/* Keepalives carry no data */
      BGP_TRACE(D_PACKETS, "Sending KEEPALIVE");
      bgp_start_timer(conn->keepalive_timer, conn->keepalive_time);
    }
  else if (s & (1 << PKT_OPEN))
    {
      s &= ~(1 << PKT_OPEN);
      type = PKT_OPEN;
      end = bgp_create_open(conn, pkt);
    }
  else if (s & (1 << PKT_ROUTE_REFRESH))
    {
      s &= ~(1 << PKT_ROUTE_REFRESH);
      type = PKT_ROUTE_REFRESH;
      end = bgp_create_route_refresh(conn, pkt);
    }
  else if (s & (1 << PKT_UPDATE))
    {
      end = bgp_create_update(conn, pkt);
      type = PKT_UPDATE;

      if (!end)
	{
	  conn->packets_to_send = 0;

	  if (!p->send_end_mark)
	    return 0;

	  p->send_end_mark = 0;
	  end = bgp_create_end_mark(conn, pkt);
	}
    }
  else
    return 0;
  conn->packets_to_send = s;
  bgp_create_header(buf, end - buf, type);
  return sk_send(sk, end - buf);
}
