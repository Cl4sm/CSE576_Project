void
wmmbp_routine(int argc, char **argv)
{
  int nfds;
  struct pollfd fds[2];

  int t_fd;
  uint64_t ticks;

  int ret;

  XEvent Event;

  /* X */
  fds[0].fd = x_fd;
  fds[0].events = POLLIN;
  nfds = 1;

  /* DBus */
  if (conn != NULL)
    {
      if (dbus_connection_get_unix_fd(conn, &fds[1].fd))
	nfds = 2;
    }
  fds[1].events = POLLIN;

  t_fd = -1;

  wmmbp_get_values();

  RedrawWindow();

  running = 1;
  while (running)
    {
      if ((conn == NULL) && (t_fd == -1))
	{
	  /* setup reconnect timer */
	  t_fd = mbp_create_timer(200);

	  if (t_fd != -1)
	    {
	      fds[1].fd = t_fd;
	      nfds = 2;
	    }
	  else
	    nfds = 1;

	  fds[1].revents = 0;
	}

      ret = poll(fds, nfds, -1);

      if (ret < 0)
	continue;

      /* DBus */
      if ((nfds == 2) && (fds[1].revents != 0))
	{
	  /* reconnection */
	  if (conn == NULL)
	    {
	      /* handle timer & reconnect, fd */
	      read(fds[1].fd, &ticks, sizeof(ticks));

	      if (wmmbp_dbus_init() == 0)
		{
		  close(t_fd);
		  t_fd = -1;

		  if (!dbus_connection_get_unix_fd(conn, &fds[1].fd))
		    {
		      fds[1].fd = -1;
		      nfds = 1;
		    }
		}
	    }
	  else /* events */
	    {
	      mbp_dbus_listen();
	    }
	}

      if ((mbpdisplay & DISPLAY_TYPE_NO_DATA) && (conn != NULL))
	wmmbp_get_values();

      /* X Events */
      if (fds[0].revents != 0)
	{
	  while (XPending(display))
	    {
	      XNextEvent(display, &Event);
	      switch (Event.type)
		{
		  case Expose:
		    mbpdisplay |= DISPLAY_FLAG_UPDATE;
		    break;

		  case DestroyNotify:
		    XCloseDisplay(display);
		    return;

		  case ButtonPress:
		    if (DISPLAY_TYPE(mbpdisplay) > DISPLAY_TYPE_NO_DATA)
		      {
			switch (DISPLAY_TYPE(mbpdisplay))
			  {
			    case DISPLAY_TYPE_MACBOOK:
			      mbpdisplay = DISPLAY_FLAG_UPDATE | DISPLAY_TYPE_AMBIENT;
			      break;

			    case DISPLAY_TYPE_AMBIENT:
			      mbpdisplay = DISPLAY_FLAG_UPDATE | DISPLAY_TYPE_MACBOOK;
			      break;
			  }
		      }
		    break;
		}
	    }
	}

      /* Update display */
      if (mbpdisplay & DISPLAY_FLAG_UPDATE)
	{
	  DisplayMBPStatus();
	  RedrawWindow();
	}
    }
}