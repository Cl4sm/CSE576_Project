int
config_commit(struct config *c, int type, int timeout)
{
  if (shutting_down)
    {
      config_free(c);
      return CONF_SHUTDOWN;
    }

  undo_available = 1;
  if (timeout > 0)
    tm_start(config_timer, timeout);
  else
    tm_stop(config_timer);

  if (configuring)
    {
      if (future_cftype)
	{
	  log(L_INFO "Queueing new configuration, ignoring the one already queued");
	  config_free(future_config);
	}
      else
	log(L_INFO "Queueing new configuration");

      future_cftype = type;
      future_config = c;
      return CONF_QUEUED;
    }

  if (config_do_commit(c, type))
    {
      config_done(NULL);
      return CONF_DONE;
    }
  return CONF_PROGRESS;
}
