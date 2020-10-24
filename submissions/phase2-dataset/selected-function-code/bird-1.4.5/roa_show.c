void
roa_show(struct roa_show_data *d)
{
  struct roa_node *rn;
  ip_addr px;
  int len;

  switch (d->mode)
    {
    case ROA_SHOW_ALL:
    case ROA_SHOW_IN:
      FIB_ITERATE_INIT(&d->fit, &d->table->fib);
      this_cli->cont = roa_show_cont;
      this_cli->cleanup = roa_show_cleanup;
      this_cli->rover = d;
      break;

    case ROA_SHOW_PX:
      rn = fib_find(&d->table->fib, &d->prefix, d->pxlen);
      if (rn)
	{
	  roa_show_node(this_cli, rn, 0, d->asn);
	  cli_msg(0, "");
	}
      else
	cli_msg(-8001, "Network not in table");
      break;

    case ROA_SHOW_FOR:
      for (len = d->pxlen; len >= 0; len--)
	{
	  px = ipa_and(d->prefix, ipa_mkmask(len));
	  rn = fib_find(&d->table->fib, &px, len);

	  if (!rn)
	    continue;

	  roa_show_node(this_cli, rn, 0, d->asn);
	}
      cli_msg(0, "");
      break;
    }
}
