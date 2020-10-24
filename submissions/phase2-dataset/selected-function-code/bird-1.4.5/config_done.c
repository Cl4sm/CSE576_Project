static void
config_done(void *unused UNUSED)
{
  if (config->shutdown)
    sysdep_shutdown_done();

  configuring = 0;
  if (old_config)
    log(L_INFO "Reconfigured");

  if (future_cftype)
    {
      int type = future_cftype;
      struct config *conf = future_config;
      future_cftype = RECONFIG_NONE;
      future_config = NULL;

      log(L_INFO "Reconfiguring to queued configuration");
      if (config_do_commit(conf, type))
	config_done(NULL);
    }
}
