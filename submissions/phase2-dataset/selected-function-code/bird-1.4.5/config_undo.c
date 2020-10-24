int
config_undo(void)
{
  if (shutting_down)
    return CONF_SHUTDOWN;

  if (!undo_available || !old_config)
    return CONF_NOTHING;

  undo_available = 0;
  tm_stop(config_timer);

  if (configuring)
    {
      if (future_cftype)
	{
	  config_free(future_config);
	  future_config = NULL;

	  log(L_INFO "Removing queued configuration");
	  future_cftype = RECONFIG_NONE;
	  return CONF_UNQUEUED;
	}
      else
	{
	  log(L_INFO "Queueing undo configuration");
	  future_cftype = RECONFIG_UNDO;
	  return CONF_QUEUED;
	}
    }

  if (config_do_commit(NULL, RECONFIG_UNDO))
    {
      config_done(NULL);
      return CONF_DONE;
    }
  return CONF_PROGRESS;
}
