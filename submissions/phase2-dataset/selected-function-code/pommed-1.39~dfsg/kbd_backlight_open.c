static int
kbd_backlight_open(int flags)
{
  char *kbdbck_node[] =
    {
      "/sys/class/leds/smc::kbd_backlight/brightness", /* 2.6.25 & up */
      "/sys/class/leds/smc:kbd_backlight/brightness"
    };
  int fd;
  int i;

  for (i = 0; i < sizeof(kbdbck_node) / sizeof(*kbdbck_node); i++)
    {
      logdebug("Trying %s\n", kbdbck_node[i]);

      fd = open(kbdbck_node[i], flags);
      if (fd >= 0)
	return fd;

      if (errno == ENOENT)
	continue;

      logmsg(LOG_WARNING, "Could not open %s: %s", kbdbck_node[i], strerror(errno));
      return -1;
    }

  return -1;
}