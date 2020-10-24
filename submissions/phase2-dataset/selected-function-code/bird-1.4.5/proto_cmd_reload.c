proto_cmd_reload(struct proto *p, unsigned int dir, int cnt UNUSED)
{
  if (p->disabled)
    {
      cli_msg(-8, "%s: already disabled", p->name);
      return;
    }

  /* If the protocol in not UP, it has no routes */
  if (p->proto_state != PS_UP)
    return;

  log(L_INFO "Reloading protocol %s", p->name);

  /* re-importing routes */
  if (dir != CMD_RELOAD_OUT)
    {
      if (! (p->reload_routes && p->reload_routes(p)))
	{
	  cli_msg(-8006, "%s: reload failed", p->name);
	  return;
	}

      /*
       * Should be done before reload_routes() hook?
       * Perhaps, but these hooks work asynchronously.
       */
      if (!p->proto->multitable)
	{
	  proto_reset_limit(p->main_ahook->rx_limit);
	  proto_reset_limit(p->main_ahook->in_limit);
	}
    }

  /* re-exporting routes */
  if (dir != CMD_RELOAD_IN)
    proto_request_feeding(p);

  cli_msg(-15, "%s: reloading", p->name);
}
