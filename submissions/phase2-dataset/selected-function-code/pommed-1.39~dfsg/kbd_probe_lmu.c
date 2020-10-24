static int
kbd_probe_lmu(void)
{
  int fd;
  int ret;
  char buffer[4];

  ret = kbd_get_lmuaddr();
  if (ret < 0)
    return -1;

  ret = kbd_get_i2cdev();
  if (ret < 0)
    return -1;

  fd = open(lmu_info.i2cdev, O_RDWR);
  if (fd < 0)
    {
      logmsg(LOG_WARNING, "Could not open device %s: %s", lmu_info.i2cdev, strerror(errno));

      return -1;
    }

  ret = ioctl(fd, I2C_SLAVE, lmu_info.lmuaddr);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "ioctl failed on %s: %s", lmu_info.i2cdev, strerror(errno));

      close(fd);
      return -1;
    }

  ret = read(fd, buffer, 4);
  if (ret != 4)
    {
      logmsg(LOG_WARNING, "Probing failed on %s: %s", lmu_info.i2cdev, strerror(errno));

      close(fd);
      return -1;
    }
  close(fd);

  logdebug("Probing successful on %s\n", lmu_info.i2cdev);

  return 0;
}