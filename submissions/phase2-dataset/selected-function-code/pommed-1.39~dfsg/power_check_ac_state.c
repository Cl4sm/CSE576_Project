static void
power_check_ac_state(int id, uint64_t ticks)
{
  int ac_state;

  ac_state = check_ac_state();

  if (ac_state == prev_state)
    return;
  else
      prev_state = ac_state;

  switch (ac_state)
    {
      case AC_STATE_ONLINE:
	logdebug("power: switched to AC\n");
	mops->lcd_backlight_toggle(LCD_ON_AC_LEVEL);
	break;

      case AC_STATE_OFFLINE:
	logdebug("power: switched to battery\n");
	mops->lcd_backlight_toggle(LCD_ON_BATT_LEVEL);
	break;

      case AC_STATE_ERROR:
	logmsg(LOG_ERR, "power: error reading AC state");
	break;

      case AC_STATE_UNKNOWN:
	logmsg(LOG_INFO, "power: unknown AC state");
	break;
    }
}