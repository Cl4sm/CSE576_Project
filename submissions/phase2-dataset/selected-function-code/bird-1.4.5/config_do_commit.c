static int
config_do_commit(struct config *c, int type)
{
  if (type == RECONFIG_UNDO)
    {
      c = old_config;
      type = old_cftype;
    }
  else
    config_free(old_config);

  old_config = config;
  old_cftype = type;
  config = c;

  configuring = 1;
  if (old_config && !config->shutdown)
    log(L_INFO "Reconfiguring");

  /* This should not be necessary, but it seems there are some
     functions that access new_config instead of config */
  new_config = config;

  if (old_config)
    old_config->obstacle_count++;

  DBG("sysdep_commit\n");
  int force_restart = sysdep_commit(c, old_config);
  DBG("global_commit\n");
  force_restart |= global_commit(c, old_config);
  DBG("rt_commit\n");
  rt_commit(c, old_config);
  roa_commit(c, old_config);
  DBG("protos_commit\n");
  protos_commit(c, old_config, force_restart, type);

  /* Just to be sure nobody uses that now */
  new_config = NULL;

  int obs = 0;
  if (old_config)
    obs = --old_config->obstacle_count;

  DBG("do_commit finished with %d obstacles remaining\n", obs);
  return !obs;
}
