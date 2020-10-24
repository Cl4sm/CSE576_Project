void
kbd_set_fnmode(void)
{
  char *fnmode_node[] =
    {
      "/sys/module/hid_apple/parameters/fnmode", /* 2.6.28 & up */
      "/sys/module/hid/parameters/pb_fnmode",    /* 2.6.20 & up */
      "/sys/module/usbhid/parameters/pb_fnmode"
    };
  FILE *fp;
  int i;

  if ((general_cfg.fnmode < 1) || (general_cfg.fnmode > 2))
    general_cfg.fnmode = 1;

  for (i = 0; i < sizeof(fnmode_node) / sizeof(*fnmode_node); i++)
    {
      logdebug("Trying %s\n", fnmode_node[i]);

      fp = fopen(fnmode_node[i], "a");
      if (fp != NULL)
	break;

      if (errno == ENOENT)
	continue;

      logmsg(LOG_INFO, "Could not open %s: %s", fnmode_node[i], strerror(errno));
      return;
    }

  if (!fp)
    {
      logmsg(LOG_INFO, "Could not set fnmode: no sysfs node found!");
      return;
    }

  fprintf(fp, "%d", general_cfg.fnmode);

  fclose(fp);
}