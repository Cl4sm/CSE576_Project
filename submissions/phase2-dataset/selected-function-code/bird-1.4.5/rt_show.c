void
rt_show(struct rt_show_data *d)
{
  net *n;

  /* Default is either a master table or a table related to a respective protocol */
  if (!d->table && d->export_protocol) d->table = d->export_protocol->table;
  if (!d->table && d->show_protocol) d->table = d->show_protocol->table;
  if (!d->table) d->table = config->master_rtc->table;

  /* Filtered routes are neither exported nor have sensible ordering */
  if (d->filtered && (d->export_mode || d->primary_only))
    cli_msg(0, "");

  if (d->pxlen == 256)
    {
      FIB_ITERATE_INIT(&d->fit, &d->table->fib);
      this_cli->cont = rt_show_cont;
      this_cli->cleanup = rt_show_cleanup;
      this_cli->rover = d;
    }
  else
    {
      if (d->show_for)
	n = net_route(d->table, d->prefix, d->pxlen);
      else
	n = net_find(d->table, d->prefix, d->pxlen);
      if (n)
	{
	  rt_show_net(this_cli, n, d);
	  cli_msg(0, "");
	}
      else
	cli_msg(8001, "Network not in table");
    }
}
