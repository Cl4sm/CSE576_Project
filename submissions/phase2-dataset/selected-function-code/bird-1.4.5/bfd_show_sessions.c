void
bfd_show_sessions(struct proto *P)
{
  byte tbuf[TM_DATETIME_BUFFER_SIZE];
  struct bfd_proto *p = (struct bfd_proto *) P;
  uint state, diag UNUSED;
  u32 tx_int, timeout;
  const char *ifname;

  if (p->p.proto_state != PS_UP)
  {
    cli_msg(-1020, "%s: is not up", p->p.name);
    cli_msg(0, "");
    return;
  }

  cli_msg(-1020, "%s:", p->p.name);
  cli_msg(-1020, "%-25s %-10s %-10s %-10s  %8s %8s",
	  "IP address", "Interface", "State", "Since", "Interval", "Timeout");


  HASH_WALK(p->session_hash_id, next_id, s)
  {
    /* FIXME: this is thread-unsafe, but perhaps harmless */
    state = s->loc_state;
    diag = s->loc_diag;
    ifname = (s->ifa && s->ifa->sk->iface) ? s->ifa->sk->iface->name : "---";
    tx_int = s->last_tx ? (MAX(s->des_min_tx_int, s->rem_min_rx_int) TO_MS) : 0;
    timeout = (MAX(s->req_min_rx_int, s->rem_min_tx_int) TO_MS) * s->rem_detect_mult;

    state = (state < 4) ? state : 0;
    tm_format_datetime(tbuf, &config->tf_proto, s->last_state_change);

    cli_msg(-1020, "%-25I %-10s %-10s %-10s  %3u.%03u  %3u.%03u",
	    s->addr, ifname, bfd_state_names[state], tbuf,
	    tx_int / 1000, tx_int % 1000, timeout / 1000, timeout % 1000);
  }
  HASH_WALK_END;

  cli_msg(0, "");
}
