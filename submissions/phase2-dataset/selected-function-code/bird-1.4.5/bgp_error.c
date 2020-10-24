bgp_error(struct bgp_conn *c, unsigned code, unsigned subcode, byte *data, int len)
{
  struct bgp_proto *p = c->bgp;

  if (c->state == BS_CLOSE)
    return;

  bgp_log_error(p, BE_BGP_TX, "Error", code, subcode, data, (len > 0) ? len : -len);
  bgp_store_error(p, c, BE_BGP_TX, (code << 16) | subcode);
  bgp_conn_enter_close_state(c);

  c->notify_code = code;
  c->notify_subcode = subcode;
  c->notify_data = data;
  c->notify_size = (len > 0) ? len : 0;
  bgp_schedule_packet(c, PKT_NOTIFICATION);

  if (code != 6)
    {
      bgp_update_startup_delay(p);
      bgp_stop(p, 0);
    }
}
