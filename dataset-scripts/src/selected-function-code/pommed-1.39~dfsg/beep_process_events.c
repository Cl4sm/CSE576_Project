void
beep_process_events(int fd, uint32_t events)
{
  int ret;

  struct input_event ev;

  if (events & (EPOLLERR | EPOLLHUP))
    {
      logmsg(LOG_WARNING, "Beeper device lost; this should not happen");

      ret = evloop_remove(fd);
      if (ret < 0)
	logmsg(LOG_ERR, "Could not remove beeper device from event loop");

      beep_close_device();

      return;
    }

  ret = read(fd, &ev, sizeof(struct input_event));

  if (ret != sizeof(struct input_event))
    return;

  if (ev.type == EV_SND)
    {
      if ((ev.code == SND_TONE) && (ev.value > 0))
	{
	  logdebug("\nBEEP: BEEP!\n");

	  beep_beep(); /* Catch that, Coyote */
	}
    }
}