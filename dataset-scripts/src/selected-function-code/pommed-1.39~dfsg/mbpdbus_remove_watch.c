static void
mbpdbus_remove_watch(DBusWatch *watch, void *data)
{
  uint32_t events;
  int fd;
  int ret;

  struct pommed_watch *w;
  struct pommed_watch *p;

  logdebug("DBus remove watch %p\n", watch);

  fd = dbus_watch_get_unix_fd(watch);
  events = 0;

  for (p = NULL, w = watches; w != NULL; p = w, w = w->next)
    {
      if (w->watch == watch)
	{
	  if (p != NULL)
	    p->next = w->next;
	  else
	    watches = w->next;

	  free(w);

	  continue;
	}

      if (w->enabled && (w->fd == fd))
	events |= w->events;
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