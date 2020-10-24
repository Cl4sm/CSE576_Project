cli_command(struct cli *c)
{
  struct config f;
  int res;

  if (config->cli_debug > 1)
    log(L_TRACE "CLI: %s", c->rx_buf);
  bzero(&f, sizeof(f));
  f.mem = c->parser_pool;
  cf_read_hook = cli_cmd_read_hook;
  cli_rh_pos = c->rx_buf;
  cli_rh_len = strlen(c->rx_buf);
  cli_rh_trick_flag = 0;
  this_cli = c;
  lp_flush(c->parser_pool);
  res = cli_parse(&f);
  if (!res)
    cli_printf(c, 9001, f.err_msg);
}
