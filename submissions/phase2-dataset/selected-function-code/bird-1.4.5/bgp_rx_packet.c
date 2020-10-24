bgp_rx_packet(struct bgp_conn *conn, byte *pkt, unsigned len)
{
  byte type = pkt[18];

  DBG("BGP: Got packet %02x (%d bytes)\n", type, len);

  if (conn->bgp->p.mrtdump & MD_MESSAGES)
    mrt_dump_bgp_packet(conn, pkt, len);

  switch (type)
    {
    case PKT_OPEN:		return bgp_rx_open(conn, pkt, len);
    case PKT_UPDATE:		return bgp_rx_update(conn, pkt, len);
    case PKT_NOTIFICATION:      return bgp_rx_notification(conn, pkt, len);
    case PKT_KEEPALIVE:		return bgp_rx_keepalive(conn);
    case PKT_ROUTE_REFRESH:	return bgp_rx_route_refresh(conn, pkt, len);
    default:			bgp_error(conn, 1, 3, pkt+18, 1);
    }
}
