static void
adb_write_kbd_value(int fd, unsigned char val)
{
  int ret;
  unsigned char buf[ADB_BUFFER_SIZE];

  buf[0] = PMU_PACKET;
  buf[1] = 0x4f; /* PMU command */
  buf[2] = 0;
  buf[3] = 0;
  buf[4] = val;

  ret = write(fd, buf, 5);
  if (ret != 5)
    {
      logmsg(LOG_ERR, "Could not set PMU kbd brightness: %s", strerror(errno));
    }
  else
    {
      ret = read(fd, buf, ADB_BUFFER_SIZE);
      if (ret < 0)
	logmsg(LOG_ERR, "Could not read PMU reply: %s", strerror(errno));
    }
}