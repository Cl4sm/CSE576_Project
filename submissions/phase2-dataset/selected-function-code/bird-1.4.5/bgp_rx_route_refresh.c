bgp_rx_route_refresh(struct bgp_conn *conn, byte *pkt, int len)
{
  struct bgp_proto *p = conn->bgp;

  BGP_TRACE(D_PACKETS, "Got ROUTE-REFRESH");

  if (conn->state != BS_ESTABLISHED)
    { bgp_error(conn, 5, fsm_err_subcode[conn->state], NULL, 0); return; }

  if (!p->cf->enable_refresh)
    { bgp_error(conn, 1, 3, pkt+18, 1); return; }

  if (len != (BGP_HEADER_LENGTH + 4))
    { bgp_error(conn, 1, 2, pkt+16, 2); return; }

  /* FIXME - we ignore AFI/SAFI values, as we support
     just one value and even an error code for an invalid
     request is not defined */

  proto_request_feeding(&p->p);
}
