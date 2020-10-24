static dbus_bool_t
mbpdbus_add_watch(DBusWatch *watch, void *data)
{
  uint32_t events;
  int fd;
  int ret;

  struct pommed_watch *w;

  logdebug("DBus add watch\n");

  fd = dbus_watch_get_unix_fd(watch);

  events = 0;
  for (w = watches; w != NULL; w = w->next)
    {
      if (w->enabled && (w->fd == fd))
	events |= w->events;
    }

  if (events != 0)
    {
      ret = evloop_remove(fd);
      if (ret < 0)
	{
	  logmsg(LOG_ERR, "Could not remove previous watch on same fd");

	  return FALSE;
	}
    }

  w = (struct pommed_watch *)malloc(sizeof(struct pommed_watch));
  if (w == NULL)
    {
      logmsg(LOG_ERR, "Could not allocate memory for a new DBus watch");

      return FALSE;
    }

  w->watch = watch;
  w->fd = fd;
  w->enabled = 1;

  w->events = dbus_to_epoll(dbus_watch_get_flags(watch));
  w->events |= EPOLLERR | EPOLLHUP;

  events |= w->events;

  ret = evloop_add(fd, events, mbpdbus_process_watch);
  if (ret < 0)
    {
      free(w);

      return FALSE;
    }

  w->next = watches;
  watches = w;

  return TRUE;
}