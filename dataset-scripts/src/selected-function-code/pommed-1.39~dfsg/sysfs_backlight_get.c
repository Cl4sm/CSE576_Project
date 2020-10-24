static int
sysfs_backlight_get(void)
{
  int fd;
  int n;
  char buffer[8];

  if (bck_driver == SYSFS_DRIVER_NONE)
    return 0;

  fd = open(actual_brightness[bck_driver], O_RDONLY);
  if (fd < 0)
    {
      logmsg(LOG_WARNING, "Could not open sysfs actual_brightness node: %s", strerror(errno));

      return 0;
    }

  memset(buffer, 0, sizeof(buffer));
  n = read(fd, buffer, sizeof(buffer) - 1);
  if (n < 1)
    {
      logmsg(LOG_WARNING, "Could not read sysfs actual_brightness node");

      close(fd);
      return 0;
    }
  close(fd);

  return atoi(buffer);
}