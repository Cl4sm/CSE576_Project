static int
kbd_backlight_get(void)
{
  int fd;
  int ret;
  char buf[8];

  fd = kbd_backlight_open(O_RDONLY);
  if (fd < 0)
    return -1;

  memset(buf, 0, 8);

  ret = read(fd, buf, 8);

  close(fd);

  if ((ret < 1) || (ret > 7))
    return -1;

  ret = atoi(buf);

  logdebug("KBD backlight value is %d\n", ret);

  if ((ret < KBD_BACKLIGHT_OFF) || (ret > KBD_BACKLIGHT_MAX))
    ret = -1;

  return ret;
}