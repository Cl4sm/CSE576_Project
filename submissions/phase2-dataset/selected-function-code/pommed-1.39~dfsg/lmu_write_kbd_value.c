static void
lmu_write_kbd_value(int fd, unsigned char val)
{
  unsigned char buf[3];

  buf[0] = 0x01;   /* i2c register */

  /* The format appears to be: (taken from pbbuttonsd)
   *          byte 1   byte 2
   *         |<---->| |<---->|
   *         xxxx7654 3210xxxx
   *             |<----->|
   *                 ^-- brightness
   */

  buf[1] = val >> 4;
  buf[2] = val << 4;

  if (write (fd, buf, 3) < 0)
    logmsg(LOG_ERR, "Could not set LMU kbd brightness: %s", strerror(errno));
}