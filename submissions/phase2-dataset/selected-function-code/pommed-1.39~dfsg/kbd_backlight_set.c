static void
kbd_backlight_set(int val, int who)
{
  int curval;

  int i;
  float fadeval;
  float step;
  struct timespec fade_step;

  int fd;
  FILE *fp;

  if (kbd_bck_info.inhibit & ~KBD_INHIBIT_CFG)
    return;

  curval = kbd_backlight_get();

  if (val == curval)
    return;

  if ((val < KBD_BACKLIGHT_OFF) || (val > KBD_BACKLIGHT_MAX))
    return;

  if (who == KBD_AUTO)
    {
      fade_step.tv_sec = 0;
      fade_step.tv_nsec = (KBD_BACKLIGHT_FADE_LENGTH / KBD_BACKLIGHT_FADE_STEPS) * 1000000;

      fadeval = (float)curval;
      step = (float)(val - curval) / (float)KBD_BACKLIGHT_FADE_STEPS;

      for (i = 0; i < KBD_BACKLIGHT_FADE_STEPS; i++)
	{
	  fadeval += step;

	  fd = kbd_backlight_open(O_WRONLY);
	  if (fd < 0)
	    continue;

	  fp = fdopen(fd, "a");
	  if (fp == NULL)
	    {
	      logmsg(LOG_WARNING, "Could not fdopen backlight fd: %s", strerror(errno));
	      close(fd);
	      continue;
	    }

	  fprintf(fp, "%d", (int)fadeval);

	  fclose(fp);

	  logdebug("KBD backlight value faded to %d\n", (int)fadeval);

	  nanosleep(&fade_step, NULL);
	}
    }

  fd = kbd_backlight_open(O_WRONLY);
  if (fd < 0)
    return;

  fp = fdopen(fd, "a");
  if (fp == NULL)
    {
      logmsg(LOG_WARNING, "Could not fdopen backlight fd %d: %s", fd, strerror(errno));
      close(fd);
      return;
    }

  fprintf(fp, "%d", val);

  fclose(fp);

  logdebug("KBD backlight value set to %d\n", val);

  mbpdbus_send_kbd_backlight(val, curval, who);

  kbd_bck_info.level = val;
}