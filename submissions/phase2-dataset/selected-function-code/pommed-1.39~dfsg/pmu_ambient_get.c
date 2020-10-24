static void
pmu_ambient_get(int *r, int *l)
{
  int fd;
  int ret;
  char buf[ADB_BUFFER_SIZE];

  fd = open(ADB_DEVICE, O_RDWR);
  if (fd < 0)
    {
      *r = -1;
      *l = -1;

      ambient_info.right = 0;
      ambient_info.left = 0;

      logmsg(LOG_ERR, "Could not open ADB device %s: %s", ADB_DEVICE, strerror(errno));
      return;
    }

  buf[0] = PMU_PACKET;
  buf[1] = 0x4f; /* PMU command */
  buf[2] = 1;

  ret = write(fd, buf, 3);
  if (ret != 3)
    {
      *r = -1;
      *l = -1;

      ambient_info.right = 0;
      ambient_info.left = 0;

      logmsg(LOG_ERR, "Could not send PMU command: %s", strerror(errno));
      close(fd);
      return;
    }

  ret = read(fd, buf, ADB_BUFFER_SIZE);
  if (ret != 5)
    {
      *r = -1;
      *l = -1;

      ambient_info.right = 0;
      ambient_info.left = 0;

      logmsg(LOG_ERR, "Could not read PMU reply: %s", strerror(errno));
      close(fd);
      return;
    }

  close(fd);
  
  /* Taken from pbbuttonsd */
  *l = (int) (((buf[2] << 8) | buf[1]) * KBD_AMBIENT_MAX) / PMU_AMBIENT_MAX_RAW;
  *r = (int) (((buf[4] << 8) | buf[3]) * KBD_AMBIENT_MAX) / PMU_AMBIENT_MAX_RAW;

  logdebug("Ambient light: right %d, left %d\n", *r, *l);

  ambient_info.right = *r;
  ambient_info.left = *l;
}