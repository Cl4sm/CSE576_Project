bgp_show_proto_info(struct proto *P)
{
  struct bgp_proto *p = (struct bgp_proto *) P;
  struct bgp_conn *c = p->conn;

  proto_show_basic_info(P);

  cli_msg(-1006, "  BGP state:          %s", bgp_state_dsc(p));
  cli_msg(-1006, "    Neighbor address: %I%J", p->cf->remote_ip, p->cf->iface);
  cli_msg(-1006, "    Neighbor AS:      %u", p->remote_as);

  if (p->gr_active)
    cli_msg(-1006, "    Neighbor graceful restart active");

  if (P->proto_state == PS_START)
    {
      struct bgp_conn *oc = &p->outgoing_conn;

      if ((p->start_state < BSS_CONNECT) &&
	  (p->startup_timer->expires))
	cli_msg(-1006, "    Error wait:       %d/%d",
		p->startup_timer->expires - now, p->startup_delay);

      if ((oc->state == BS_ACTIVE) &&
	  (oc->connect_retry_timer->expires))
	cli_msg(-1006, "    Start delay:      %d/%d",
		oc->connect_retry_timer->expires - now, p->cf->start_delay_time);

      if (p->gr_active && p->gr_timer->expires)
	cli_msg(-1006, "    Restart timer:    %d/-", p->gr_timer->expires - now);
    }
  else if (P->proto_state == PS_UP)
    {
      cli_msg(-1006, "    Neighbor ID:      %R", p->remote_id);
      cli_msg(-1006, "    Neighbor caps:   %s%s%s%s%s",
	      c->peer_refresh_support ? " refresh" : "",
	      c->peer_gr_able ? " restart-able" : (c->peer_gr_aware ? " restart-aware" : ""),
	      c->peer_as4_support ? " AS4" : "",
	      (c->peer_add_path & ADD_PATH_RX) ? " add-path-rx" : "",
	      (c->peer_add_path & ADD_PATH_TX) ? " add-path-tx" : "");
      cli_msg(-1006, "    Session:          %s%s%s%s%s%s%s",
	      p->is_internal ? "internal" : "external",
	      p->cf->multihop ? " multihop" : "",
	      p->rr_client ? " route-reflector" : "",
	      p->rs_client ? " route-server" : "",
	      p->as4_session ? " AS4" : "",
	      p->add_path_rx ? " add-path-rx" : "",
	      p->add_path_tx ? " add-path-tx" : "");
      cli_msg(-1006, "    Source address:   %I", p->source_addr);
      if (P->cf->in_limit)
	cli_msg(-1006, "    Route limit:      %d/%d",
		p->p.stats.imp_routes + p->p.stats.filt_routes, P->cf->in_limit->limit);
      cli_msg(-1006, "    Hold timer:       %d/%d",
	      tm_remains(c->hold_timer), c->hold_time);
      cli_msg(-1006, "    Keepalive timer:  %d/%d",
	      tm_remains(c->keepalive_timer), c->keepalive_time);
    }

  if ((p->last_error_class != BE_NONE) && 
      (p->last_error_class != BE_MAN_DOWN))
    {
      const char *err1 = bgp_err_classes[p->last_error_class];
      const char *err2 = bgp_last_errmsg(p);
      cli_msg(-1006, "    Last error:       %s%s", err1, err2);
    }
}
