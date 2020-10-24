cmd_reconfig(char *name, int type, int timeout)
{
  if (cli_access_restricted())
    return;

  struct config *conf = cmd_read_config(name);
  if (!conf)
    return;

  int r = config_commit(conf, type, timeout);

  if ((r >= 0) && (timeout > 0))
    {
      cmd_reconfig_stored_cli = this_cli;
      cli_msg(-22, "Undo scheduled in %d s", timeout);
    }

  cmd_reconfig_msg(r);
}
