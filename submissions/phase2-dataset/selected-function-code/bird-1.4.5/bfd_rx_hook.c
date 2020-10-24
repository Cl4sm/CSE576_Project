static int
bfd_rx_hook(sock *sk, int len)
{
  struct bfd_proto *p =  sk->data;
  struct bfd_ctl_packet *pkt = (struct bfd_ctl_packet *) sk->rbuf;
  const char *err_dsc = NULL;
  uint err_val = 0;
  char fb[8];

  if ((sk->sport == BFD_CONTROL_PORT) && (sk->rcv_ttl < 255))
    DROP("wrong TTL", sk->rcv_ttl);

  if (len < BFD_BASE_LEN)
    DROP("too short", len);

  u8 version = bfd_pkt_get_version(pkt);
  if (version != 1)
    DROP("version mismatch", version);

  if ((pkt->length < BFD_BASE_LEN) || (pkt->length > len))
    DROP("length mismatch", pkt->length);

  if (pkt->detect_mult == 0)
    DROP("invalid detect mult", 0);

  if ((pkt->flags & BFD_FLAG_MULTIPOINT) ||
      ((pkt->flags & BFD_FLAG_POLL) && (pkt->flags & BFD_FLAG_FINAL)))
    DROP("invalid flags", pkt->flags);

  if (pkt->snd_id == 0)
    DROP("invalid my discriminator", 0);

  struct bfd_session *s;
  u32 id = ntohl(pkt->rcv_id);

  if (id)
  {
    s = bfd_find_session_by_id(p, id);

    if (!s)
      DROP("unknown session id", id);
  }
  else
  {
    u8 ps = bfd_pkt_get_state(pkt);
    if (ps > BFD_STATE_DOWN)
      DROP("invalid init state", ps);
      
    s = bfd_find_session_by_addr(p, sk->faddr);

    /* FIXME: better session matching and message */
    if (!s)
      return 1;
  }

  /* FIXME: better authentication handling and message */
  if (pkt->flags & BFD_FLAG_AP)
    DROP("authentication not supported", 0);


  u32 old_tx_int = s->des_min_tx_int;
  u32 old_rx_int = s->rem_min_rx_int;

  s->rem_id= ntohl(pkt->snd_id);
  s->rem_state = bfd_pkt_get_state(pkt);
  s->rem_diag = bfd_pkt_get_diag(pkt);
  s->rem_demand_mode = pkt->flags & BFD_FLAG_DEMAND;
  s->rem_min_tx_int = ntohl(pkt->des_min_tx_int);
  s->rem_min_rx_int = ntohl(pkt->req_min_rx_int);
  s->rem_detect_mult = pkt->detect_mult;

  TRACE(D_PACKETS, "CTL received from %I [%s%s]", sk->faddr,
	bfd_state_names[s->rem_state], bfd_format_flags(pkt->flags, fb));

  bfd_session_process_ctl(s, pkt->flags, old_tx_int, old_rx_int);
  return 1;

 drop:
  log(L_REMOTE "%s: Bad packet from %I - %s (%u)", p->p.name, sk->faddr, err_dsc, err_val);
  return 1;
}
