static void
mbpdbus_toggle_watch(DBusWatch *watch, void *data)
{
  uint32_t events;
  int fd;
  int ret;

  struct pommed_watch *w;

  logdebug("DBus toggle watch\n");

  fd = dbus_watch_get_unix_fd(watch);
  events = 0;

  for (w = watches; w != NULL; w = w->next)
    {
      if (w->watch == watch)
	{
	  if (!dbus_watch_get_enabled(watch))
	    w->enabled = 0;
	  else
	    {
	      w->enabled = 1;
	      events |= w->events;
	    }

	  continue;
	}

      if (w->enabled && (w->fd == fd))
	events |= events;
    }

  ret = evloop_remove(fd);
  if (ret < 0)
    return;

  if (events == 0)
    return;

  ret = evloop_add(fd, events, mbpdbus_process_watch);
  if (ret < 0)
    logmsg(LOG_WARNING, "Could not re-add watch");
}