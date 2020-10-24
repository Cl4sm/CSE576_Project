int
beep_init(void)
{
  int ret;

  beep_fd = -1;

  ret = beep_thread_init();
  if (ret < 0)
    {
      logmsg(LOG_ERR, "beep: thread init failed, disabling");

      beep_cfg.enabled = 0;

      return -1;
    }

  beep_thread_running = 1;

  ret = beep_open_device();
  if (ret < 0)
    return -1;

  ret = evloop_add(beep_fd, EPOLLIN, beep_process_events);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not add device to event loop");

      beep_cfg.enabled = 0;

      beep_close_device();

      return -1;
    }

  return 0;
}