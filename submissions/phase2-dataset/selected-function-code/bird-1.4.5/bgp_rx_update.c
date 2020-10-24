bgp_rx_update(struct bgp_conn *conn, byte *pkt, int len)
{
  struct bgp_proto *p = conn->bgp;
  byte *withdrawn, *attrs, *nlri;
  int withdrawn_len, attr_len, nlri_len;

  BGP_TRACE_RL(&rl_rcv_update, D_PACKETS, "Got UPDATE");

  /* Workaround for some BGP implementations that skip initial KEEPALIVE */
  if (conn->state == BS_OPENCONFIRM)
    bgp_conn_enter_established_state(conn);

  if (conn->state != BS_ESTABLISHED)
    { bgp_error(conn, 5, fsm_err_subcode[conn->state], NULL, 0); return; }
  bgp_start_timer(conn->hold_timer, conn->hold_time);

  /* Find parts of the packet and check sizes */
  if (len < 23)
    {
      bgp_error(conn, 1, 2, pkt+16, 2);
      return;
    }
  withdrawn = pkt + 21;
  withdrawn_len = get_u16(pkt + 19);
  if (withdrawn_len + 23 > len)
    goto malformed;
  attrs = withdrawn + withdrawn_len + 2;
  attr_len = get_u16(attrs - 2);
  if (withdrawn_len + attr_len + 23 > len)
    goto malformed;
  nlri = attrs + attr_len;
  nlri_len = len - withdrawn_len - attr_len - 23;
  if (!attr_len && nlri_len)
    goto malformed;
  DBG("Sizes: withdrawn=%d, attrs=%d, NLRI=%d\n", withdrawn_len, attr_len, nlri_len);

  lp_flush(bgp_linpool);

  bgp_do_rx_update(conn, withdrawn, withdrawn_len, nlri, nlri_len, attrs, attr_len);
  return;

malformed:
  bgp_error(conn, 3, 1, NULL, 0);
}
