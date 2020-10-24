void
sysfs_backlight_step(int dir)
{
  int val;
  int newval;

  if (bck_driver == SYSFS_DRIVER_NONE)
    return;

  val = sysfs_backlight_get();

  if (dir == STEP_UP)
    {
      newval = val + lcd_sysfs_cfg.step;

      if (newval > lcd_bck_info.max)
	newval = lcd_bck_info.max;

      logdebug("LCD stepping +%d -> %d\n", lcd_sysfs_cfg.step, newval);
    }
  else if (dir == STEP_DOWN)
    {
      newval = val - lcd_sysfs_cfg.step;

      if (newval < SYSFS_BACKLIGHT_OFF)
	newval = SYSFS_BACKLIGHT_OFF;

      logdebug("LCD stepping -%d -> %d\n", lcd_sysfs_cfg.step, newval);
    }
  else
    return;

  sysfs_backlight_set(newval);

  mbpdbus_send_lcd_backlight(newval, val, LCD_USER);

  lcd_bck_info.level = newval;
}