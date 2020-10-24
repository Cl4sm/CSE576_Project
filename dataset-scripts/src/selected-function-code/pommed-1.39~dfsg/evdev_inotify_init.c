static int
evdev_inotify_init(void)
{
  int ret;
  int fd;

  fd = inotify_init();
  if (fd < 0)
    {
      logmsg(LOG_ERR, "Failed to initialize inotify: %s", strerror(errno));

      return -1;
    }

  ret = inotify_add_watch(fd, EVDEV_DIR, IN_CREATE | IN_ONLYDIR);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Failed to add inotify watch for %s: %s", EVDEV_DIR, strerror(errno));

      close(fd);
      fd = -1;

      return -1;
    }

  ret = evloop_add(fd, EPOLLIN, evdev_inotify_process);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Failed to add inotify fd to event loop");

      close(fd);

      return -1;
    }

  return 0;
}