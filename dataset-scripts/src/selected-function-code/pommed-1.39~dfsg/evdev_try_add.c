static int
evdev_try_add(int fd)
{
  unsigned short id[4];
  unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
  char devname[256];

  int ret;

  devname[0] = '\0';
  ioctl(fd, EVIOCGNAME(sizeof(devname)), devname);

  logdebug("\nInvestigating evdev [%s]\n", devname);

  ioctl(fd, EVIOCGID, id);

  if ((!evdev_is_internal(id))
#ifndef __powerpc__
      && !(appleir_cfg.enabled && evdev_is_appleir(id))
#endif
      && !(has_kbd_backlight() && evdev_is_lidswitch(id))
      && !(evdev_is_mouseemu(id))
      && !(evdev_is_extkbd(id)))
    {
      logdebug("Discarding evdev: bus 0x%04x, vid 0x%04x, pid 0x%04x\n", id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT]);

      close(fd);

      return -1;
    }

  memset(bit, 0, sizeof(bit));

  ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);

  if (!test_bit(EV_KEY, bit[0]))
    {
      logdebug("evdev: no EV_KEY event type (not a keyboard)\n");

      if (!test_bit(EV_SW, bit[0]))
	{
	  logdebug("Discarding evdev: no EV_SW event type (not a switch)\n");

	  close(fd);

	  return -1;
	}
    }
  /* Wireless keyboards advertise EV_ABS events, single them out */
  else if (test_bit(EV_ABS, bit[0]) && !(evdev_is_extkbd_alu_wl(id)))
    {
      logdebug("Discarding evdev with EV_ABS event type (mouse/trackpad)\n");

      close(fd);

      return -1;
    }

  /* There are 2 keyboards, but one of them only has the eject key;
     the real keyboard has all the keys and the LEDs. Checking for
     the LEDs is a quick way of identifying the keyboard we want.
  */
  if (test_bit(EV_LED, bit[0]) && evdev_is_internal(id))
    {
      logdebug(" -> Internal keyboard\n");

      internal_kbd_fd = fd;
    }

  ret = evloop_add(fd, EPOLLIN, evdev_process_events);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not add device to event loop");

      if (fd == internal_kbd_fd)
	internal_kbd_fd = -1;

      close(fd);

      return -1;
    }

  return 0;
}