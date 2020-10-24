static void
kbd_pmu_backlight_set(int val, int who)
{
  int curval;

  int i;
  float fadeval;
  float step;
  struct timespec fade_step;

  int fd;

  if (kbd_bck_info.inhibit & ~KBD_INHIBIT_CFG)
    return;

  curval = kbd_backlight_get();

  if (val == curval)
    return;

  if ((val < KBD_BACKLIGHT_OFF) || (val > KBD_BACKLIGHT_MAX))
    return;

  fd = open(ADB_DEVICE, O_RDWR);
  if (fd < 0)
    {
      logmsg(LOG_ERR, "Could not open %s: %s", ADB_DEVICE, strerror(errno));

      return;
    }

  if (who == KBD_AUTO)
    {
      fade_step.tv_sec = 0;
      fade_step.tv_nsec = (KBD_BACKLIGHT_FADE_LENGTH / KBD_BACKLIGHT_FADE_STEPS) * 1000000;

      fadeval = (float)curval;
      step = (float)(val - curval) / (float)KBD_BACKLIGHT_FADE_STEPS;

      for (i = 0; i < KBD_BACKLIGHT_FADE_STEPS; i++)
	{
	  fadeval += step;

	  adb_write_kbd_value(fd, (unsigned char)fadeval);

	  logdebug("KBD backlight value faded to %d\n", (int)fadeval);

	  nanosleep(&fade_step, NULL);
	}
    }

  adb_write_kbd_value(fd, val);

  close(fd);

  mbpdbus_send_kbd_backlight(val, kbd_bck_info.level, who);

  kbd_bck_info.level = val;
}