static void
lmu_ambient_get(int *r, int *l)
{
  int fd;
  int ret;
  char buf[4];

  fd = open(lmu_info.i2cdev, O_RDONLY);
  if (fd < 0)
    {
      *r = -1;
      *l = -1;

      ambient_info.right = 0;
      ambient_info.left = 0;

      logmsg(LOG_ERR, "Could not open i2c device %s: %s", lmu_info.i2cdev, strerror(errno));
      return;
    }

  ret = ioctl(fd, I2C_SLAVE, lmu_info.lmuaddr);
  if (ret < 0)
    {
      close(fd);

      *r = -1;
      *l = -1;

      ambient_info.right = 0;
      ambient_info.left = 0;

      logmsg(LOG_ERR, "ioctl failed on %s: %s", lmu_info.i2cdev, strerror(errno));

      return;
    }
  
  ret = read(fd, buf, 4);
  if (ret != 4)
    {
      close(fd);

      *r = -1;
      *l = -1;

      ambient_info.right = 0;
      ambient_info.left = 0;

      return;
    }
  close(fd);

  /* found in pbbuttonsd.conf */
  *r = (int) (((buf[0] << 8) | buf[1]) * KBD_AMBIENT_MAX) / LMU_AMBIENT_MAX_RAW;
  *l = (int) (((buf[2] << 8) | buf[3]) * KBD_AMBIENT_MAX) / LMU_AMBIENT_MAX_RAW;

  logdebug("Ambient light: right %d, left %d\n", *r, *l);

  ambient_info.right = *r;
  ambient_info.left = *l;
}