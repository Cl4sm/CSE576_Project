static int
gma950_backlight_map(void)
{
  if (length == 0)
    {
      logdebug("No probing done !\n");
      return -1;
    }

  fd = open(sysfs_resource, O_RDWR);
	
  if (fd < 0)
    {
      logmsg(LOG_WARNING, "Cannot open %s: %s", sysfs_resource, strerror(errno));
      return -1;
    }

  memory = mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  if (memory == MAP_FAILED)
    {
      logmsg(LOG_ERR, "mmap failed: %s", strerror(errno));

      close(fd);
      fd = -1;

      return -1;
    }

  return 0;
}