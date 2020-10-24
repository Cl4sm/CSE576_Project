static void
mbpdbus_process_watch(int fd, uint32_t events)
{
  int flags;
  uint32_t wanted;

  DBusDispatchStatus ds;

  struct pommed_watch *w;

  logdebug("DBus process watch\n");

  for (w = watches; w != NULL; w = w->next)
    {
      if (!w->enabled)
	continue;

      if (w->fd == fd)
	{
	  wanted = events & w->events;

	  if (wanted != 0)
	    {
	      flags = epoll_to_dbus(wanted);

	      dbus_watch_handle(w->watch, flags);

	      do
		{
		  ds = dbus_connection_dispatch(conn);
		}
	      while (ds == DBUS_DISPATCH_DATA_REMAINS);

	      /* Get out of the loop, as DBus will remove the watches
	       * and our linked list can become invalid under our feet
	       */
	      if (events & (EPOLLERR | EPOLLHUP))
		break;
	    }
	}
    }
}