int
config_monitor(void)
{
  int fd;
  int ret;

  fd = inotify_init();
  if (fd < 0)
    {
      fprintf(stderr, "Error: could not initialize inotify instance: %s\n", strerror(errno));

      return -1;
    }

  ret = fcntl(fd, F_GETFL);
  if (ret < 0)
    {
      close(fd);

      fprintf(stderr, "Error: failed to get inotify fd flags: %s\n", strerror(errno));

      return -1;
    }

  ret = fcntl(fd, F_SETFL, ret | O_NONBLOCK);
  if (ret < 0)
    {
      close(fd);

      fprintf(stderr, "Error: failed to set inotify fd flags: %s\n", strerror(errno));

      return -1;
    }

  ret = inotify_add_watch(fd, conffile, IN_CLOSE_WRITE);
  if (ret < 0)
    {
      close(fd);

      fprintf(stderr, "Error: could not add inotify watch: %s\n", strerror(errno));

      return -1;
    }

  return fd;
}