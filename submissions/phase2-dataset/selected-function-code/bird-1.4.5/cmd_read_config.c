cmd_read_config(char *name)
{
  struct config *conf;

  if (!name)
    name = config_name;

  cli_msg(-2, "Reading configuration from %s", name);
  if (!unix_read_config(&conf, name))
    {
      if (conf->err_msg)
	cli_msg(8002, "%s, line %d: %s", conf->err_file_name, conf->err_lino, conf->err_msg);
      else
	cli_msg(8002, "%s: %m", name);
      config_free(conf);
      conf = NULL;
    }

  return conf;
}
