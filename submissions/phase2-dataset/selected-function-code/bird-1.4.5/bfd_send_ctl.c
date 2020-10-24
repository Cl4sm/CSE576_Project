void
bfd_send_ctl(struct bfd_proto *p, struct bfd_session *s, int final)
{
  sock *sk = s->ifa->sk;
  struct bfd_ctl_packet *pkt = (struct bfd_ctl_packet *) sk->tbuf;
  char fb[8];

  pkt->vdiag = bfd_pack_vdiag(1, s->loc_diag);
  pkt->flags = bfd_pack_flags(s->loc_state, 0);
  pkt->detect_mult = s->detect_mult;
  pkt->length = BFD_BASE_LEN;
  pkt->snd_id = htonl(s->loc_id);
  pkt->rcv_id = htonl(s->rem_id);
  pkt->des_min_tx_int = htonl(s->des_min_tx_new);
  pkt->req_min_rx_int = htonl(s->req_min_rx_new);
  pkt->req_min_echo_rx_int = 0;

  if (final)
    pkt->flags |= BFD_FLAG_FINAL;
  else if (s->poll_active)
    pkt->flags |= BFD_FLAG_POLL;

  if (sk->tbuf != sk->tpos)
    log(L_WARN "%s: Old packet overwritten in TX buffer", p->p.name);

  TRACE(D_PACKETS, "Sending CTL to %I [%s%s]", s->addr,
	bfd_state_names[s->loc_state], bfd_format_flags(pkt->flags, fb));

  sk_send_to(sk, pkt->length, s->addr, sk->dport);
}
