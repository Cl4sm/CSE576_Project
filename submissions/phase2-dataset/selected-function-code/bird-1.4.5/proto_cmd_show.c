proto_cmd_show(struct proto *p, unsigned int verbose, int cnt)
{
  byte buf[256], tbuf[TM_DATETIME_BUFFER_SIZE];

  /* First protocol - show header */
  if (!cnt)
    cli_msg(-2002, "name     proto    table    state  since       info");

  buf[0] = 0;
  if (p->proto->get_status)
    p->proto->get_status(p, buf);
  tm_format_datetime(tbuf, &config->tf_proto, p->last_state_change);
  cli_msg(-1002, "%-8s %-8s %-8s %-5s  %-10s  %s",
	  p->name,
	  p->proto->name,
	  p->table->name,
	  proto_state_name(p),
	  tbuf,
	  buf);
  if (verbose)
    {
      if (p->cf->dsc)
	cli_msg(-1006, "  Description:    %s", p->cf->dsc);
      if (p->cf->router_id)
	cli_msg(-1006, "  Router ID:      %R", p->cf->router_id);

      if (p->proto->show_proto_info)
	p->proto->show_proto_info(p);
      else
	proto_show_basic_info(p);

      cli_msg(-1006, "");
    }
}
