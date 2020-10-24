static int
beep_open_device(void)
{
  char *uinput_dev[3] =
    {
      "/dev/input/uinput",
      "/dev/uinput",
      "/dev/misc/uinput"
    };
  struct uinput_user_dev dv;
  int fd;
  int i;
  int ret;

  if (beep_cfg.enabled == 0)
    return -1;

  for (i = 0; i < (sizeof(uinput_dev) / sizeof(uinput_dev[0])); i++)
    {
      fd = open(uinput_dev[i], O_RDWR, 0);

      if (fd >= 0)
	break;
    }

  if (fd < 0)
    {
      logmsg(LOG_ERR, "beep: could not open uinput: %s", strerror(errno));
      logmsg(LOG_ERR, "beep: Do you have the uinput module loaded?");

      return -1;
    }

  memset(&dv, 0, sizeof(dv));
  strcpy(dv.name, BEEP_DEVICE_NAME);
  dv.id.bustype = BUS_VIRTUAL;
  dv.id.vendor = 0;
  dv.id.product = 0;
  dv.id.version = 1;

  ret = write(fd, &dv, sizeof(dv));
  if (ret != sizeof(dv))
    {
      logmsg(LOG_ERR, "beep: could not set device name: %s", strerror(errno));

      close(fd);
      return -1;
    }

  ret = ioctl(fd, UI_SET_EVBIT, EV_SND);
  if (ret != 0)
    {
      logmsg(LOG_ERR, "beep: could not request EV_SND: %s", strerror(errno));

      close(fd);
      return -1;
    }

  ret = ioctl(fd, UI_SET_SNDBIT, SND_BELL);
  if (ret != 0)
    {
      logmsg(LOG_ERR, "beep: could not request SND_BELL: %s", strerror(errno));

      close(fd);
      return -1;
    }

  ret = ioctl(fd, UI_SET_SNDBIT, SND_TONE);
  if (ret != 0)
    {
      logmsg(LOG_ERR, "beep: could not request SND_TONE: %s", strerror(errno));

      close(fd);
      return -1;
    }

  ret = ioctl(fd, UI_DEV_CREATE, NULL);
  if (ret != 0)
    {
      logmsg(LOG_ERR, "beep: could not create uinput device: %s", strerror(errno));

      close(fd);
      return -1;
  }

  beep_fd = fd;

  return 0;
}