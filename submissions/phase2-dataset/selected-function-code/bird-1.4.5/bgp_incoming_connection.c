static int
bgp_incoming_connection(sock *sk, int dummy UNUSED)
{
  struct proto_config *pc;

  DBG("BGP: Incoming connection from %I port %d\n", sk->daddr, sk->dport);
  WALK_LIST(pc, config->protos)
    if (pc->protocol == &proto_bgp && pc->proto)
      {
	struct bgp_proto *p = (struct bgp_proto *) pc->proto;
	if (ipa_equal(p->cf->remote_ip, sk->daddr) &&
	    (!ipa_has_link_scope(sk->daddr) || (p->cf->iface == sk->iface)))
	  {
	    /* We are in proper state and there is no other incoming connection */
	    int acc = (p->p.proto_state == PS_START || p->p.proto_state == PS_UP) &&
	      (p->start_state >= BSS_CONNECT) && (!p->incoming_conn.sk);

	    if (p->conn && (p->conn->state == BS_ESTABLISHED) && p->gr_ready)
	    {
	      bgp_store_error(p, NULL, BE_MISC, BEM_GRACEFUL_RESTART);
	      bgp_handle_graceful_restart(p);
	      bgp_conn_enter_idle_state(p->conn);
	      acc = 1;
	    }

	    BGP_TRACE(D_EVENTS, "Incoming connection from %I%J (port %d) %s",
		      sk->daddr, ipa_has_link_scope(sk->daddr) ? sk->iface : NULL,
		      sk->dport, acc ? "accepted" : "rejected");

	    if (!acc)
	      goto reject;

	    int hops = p->cf->multihop ? : 1;

	    if (sk_set_ttl(sk, p->cf->ttl_security ? 255 : hops) < 0)
	      goto err;

	    if (p->cf->ttl_security)
	      if (sk_set_min_ttl(sk, 256 - hops) < 0)
		goto err;

	    bgp_setup_conn(p, &p->incoming_conn);
	    bgp_setup_sk(&p->incoming_conn, sk);
	    bgp_send_open(&p->incoming_conn);
	    return 0;

	  err:
	    sk_log_error(sk, p->p.name);
	    log(L_ERR "%s: Incoming connection aborted", p->p.name);
	    rfree(sk);
	    return 0;
	  }
      }

  log(L_WARN "BGP: Unexpected connect from unknown address %I%J (port %d)",
      sk->daddr, ipa_has_link_scope(sk->daddr) ? sk->iface : NULL, sk->dport);
 reject:
  rfree(sk);
  return 0;
}
