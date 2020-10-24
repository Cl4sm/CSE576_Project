proto_show_basic_info(struct proto *p)
{
  // cli_msg(-1006, "  Table:          %s", p->table->name);
  cli_msg(-1006, "  Preference:     %d", p->preference);
  cli_msg(-1006, "  Input filter:   %s", filter_name(p->cf->in_filter));
  cli_msg(-1006, "  Output filter:  %s", filter_name(p->cf->out_filter));

  if (graceful_restart_state == GRS_ACTIVE)
    cli_msg(-1006, "  GR recovery:   %s%s",
	    p->gr_lock ? " pending" : "",
	    p->gr_wait ? " waiting" : "");

  proto_show_limit(p->cf->rx_limit, "Receive limit:");
  proto_show_limit(p->cf->in_limit, "Import limit:");
  proto_show_limit(p->cf->out_limit, "Export limit:");

  if (p->proto_state != PS_DOWN)
    proto_show_stats(&p->stats, p->cf->in_keep_filtered);
}
