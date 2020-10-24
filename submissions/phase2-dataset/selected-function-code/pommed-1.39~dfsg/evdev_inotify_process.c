void
evdev_inotify_process(int fd, uint32_t events)
{
  int ret;
  int efd;
  int qsize;

  struct inotify_event *all_ie;
  struct inotify_event *ie;
  char evdev[32];

  if (events & (EPOLLERR | EPOLLHUP))
    {
      logmsg(LOG_WARNING, "inotify fd lost; this should not happen");

      ret = evloop_remove(fd);
      if (ret < 0)
	logmsg(LOG_ERR, "Could not remove inotify from event loop");

      close(fd);

      return;
    }

  /* Determine the size of the inotify queue */
  ret = ioctl(fd, FIONREAD, &qsize);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not determine inotify queue size: %s", strerror(errno));

      return;
    }

  all_ie = (struct inotify_event *) malloc(qsize);
  if (all_ie == NULL)
    {
      logmsg(LOG_ERR, "Could not allocate %d bytes for inotify events", qsize);

      return;
    }

  ret = read(fd, all_ie, qsize);
  if (ret < 0)
    {
      logmsg(LOG_WARNING, "inotify read failed: %s", strerror(errno));

      free(all_ie);
      return;
    }

  /* ioctl(FIONREAD) returns the number of bytes, now we need the number of elements */
  qsize /= sizeof(struct inotify_event);

  /* Loop through all the events we got */
  for (ie = all_ie; (ie - all_ie) < qsize; ie += (1 + (ie->len / sizeof(struct inotify_event))))
    {
      /* ie[0] contains the inotify event information
       * the memory space for ie[1+] contains the name of the file
       * see the inotify documentation
       */

      if ((ie->len == 0) || (ie->name == NULL))
	{
	  logdebug("inotify event with no name\n");

	  continue;
	}

      logdebug("Found new event device %s/%s\n", EVDEV_DIR, ie->name);

      if (strncmp("event", ie->name, 5))
	{
	  logdebug("Discarding %s/%s\n", EVDEV_DIR, ie->name);

	  continue;
	}

      ret = snprintf(evdev, sizeof(evdev), "%s/%s", EVDEV_DIR, ie->name);

      if ((ret <= 0) || (ret >= sizeof(evdev)))
	continue;

      efd = open(evdev, O_RDWR);
      if (efd < 0)
	{
	  if (errno != ENOENT)
	    logmsg(LOG_WARNING, "Could not open %s: %s", evdev, strerror(errno));

	  continue;
	}

      evdev_try_add(efd);
    }

  free(all_ie);
}