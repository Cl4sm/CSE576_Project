void
evdev_process_events(int fd, uint32_t events)
{
  int ret;

  struct input_event ev;

  /* some of the event devices cease to exist when suspending */
  if (events & (EPOLLERR | EPOLLHUP))
    {
      logmsg(LOG_INFO, "Error condition signaled on event device");

      ret = evloop_remove(fd);
      if (ret < 0)
	logmsg(LOG_ERR, "Could not remove device from event loop");

      if (fd == internal_kbd_fd)
	internal_kbd_fd = -1;

      close(fd);

      return;
    }

  ret = read(fd, &ev, sizeof(struct input_event));

  if (ret != sizeof(struct input_event))
    return;

  if (ev.type == EV_KEY)
    {
      /* key released - we don't care */
      if (ev.value == 0)
	return;

      /* Reset keyboard backlight idle timer */
      if (fd == internal_kbd_fd)
	{
	  kbd_bck_info.idle = 0;
	  kbd_backlight_inhibit_clear(KBD_INHIBIT_IDLE);
	}

      switch (ev.code)
	{
	  case KEY_BRIGHTNESSDOWN:
	    logdebug("\nKEY: LCD backlight down\n");

	    mops->lcd_backlight_step(STEP_DOWN);
	    break;

	  case KEY_BRIGHTNESSUP:
	    logdebug("\nKEY: LCD backlight up\n");

	    mops->lcd_backlight_step(STEP_UP);
	    break;

	  case KEY_MUTE:
	    logdebug("\nKEY: audio mute\n");

	    audio_toggle_mute();
	    break;

	  case KEY_VOLUMEDOWN:
	    logdebug("\nKEY: audio down\n");

	    audio_step(STEP_DOWN);
	    break;

	  case KEY_VOLUMEUP:
	    logdebug("\nKEY: audio up\n");

	    audio_step(STEP_UP);
	    break;

	  case KEY_SWITCHVIDEOMODE:
	    logdebug("\nKEY: video toggle\n");

	    video_switch();
	    break;

	  case KEY_KBDILLUMTOGGLE:
	    logdebug("\nKEY: keyboard backlight off\n");

	    if (!has_kbd_backlight())
	      break;

	    if (kbd_cfg.auto_on)
	      kbd_backlight_inhibit_toggle(KBD_INHIBIT_USER);
	    else
	      kbd_backlight_toggle();
	    break;

	  case KEY_KBDILLUMDOWN:
	    logdebug("\nKEY: keyboard backlight down\n");

	    if (!has_kbd_backlight())
	      break;

	    kbd_backlight_step(STEP_DOWN);
	    if (kbd_bck_info.level == KBD_BACKLIGHT_OFF)
	      kbd_backlight_inhibit_set(KBD_INHIBIT_USER);
	    break;

	  case KEY_KBDILLUMUP:
	    logdebug("\nKEY: keyboard backlight up\n");

	    if (!has_kbd_backlight())
	      break;

	    kbd_backlight_inhibit_clear(KBD_INHIBIT_USER);
	    kbd_backlight_step(STEP_UP);
	    break;

	  case KEY_EJECTCD:
	    logdebug("\nKEY: CD eject\n");

	    cd_eject();
	    break;

	  case KEY_NEXTSONG:
	    logdebug("\nKEY: IR fast forward\n");
	    break;

	  case KEY_PREVIOUSSONG:
	    logdebug("\nKEY: IR rewind\n");
	    break;

	  case KEY_PLAYPAUSE:
	    logdebug("\nKEY: IR play/pause\n");
	    break;

	  case KEY_MENU:
	    logdebug("\nKEY: IR menu\n");
	    break;

	  default:
#if 0
	    logdebug("\nKEY: %x\n", ev.code);
#endif /* 0 */
	    break;
	}
    }
  else if (ev.type == EV_SW)
    {
      /* Lid switch */
      if (ev.code == SW_LID)
	{
	  if (ev.value)
	    {
	      logdebug("\nLID: closed\n");

	      kbd_backlight_inhibit_set(KBD_INHIBIT_LID);
	    }
	  else
	    {
	      logdebug("\nLID: open\n");

	      kbd_backlight_inhibit_clear(KBD_INHIBIT_LID);
	    }
	}
    }
}