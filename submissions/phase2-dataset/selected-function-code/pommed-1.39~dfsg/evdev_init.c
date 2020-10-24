int
evdev_init(void)
{
  int ret;
  int i;

  char evdev[32];

  int ndevs;
  int fd;

  internal_kbd_fd = -1;

  ndevs = 0;
  for (i = 0; i < EVDEV_MAX; i++)
    {
      ret = snprintf(evdev, 32, "%s%d", EVDEV_BASE, i);

      if ((ret <= 0) || (ret > 31))
	return -1;

      fd = open(evdev, O_RDWR);
      if (fd < 0)
	{
	  if (errno != ENOENT)
	    logmsg(LOG_WARNING, "Could not open %s: %s", evdev, strerror(errno));

	  continue;
	}

      if (evdev_try_add(fd) == 0)
	ndevs++;
    }

  logdebug("\nFound %d devices\n", ndevs);

  /* Initialize inotify */
  evdev_inotify_init();

  return ndevs;
}