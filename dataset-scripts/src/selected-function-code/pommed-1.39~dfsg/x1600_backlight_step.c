void
x1600_backlight_step(int dir)
{
  int ret;

  int val;
  int newval;

  ret = x1600_backlight_map();
  if (ret < 0)
    return;

  val = x1600_backlight_get();

  if (dir == STEP_UP)
    {
      newval = val + lcd_x1600_cfg.step;

      if (newval > X1600_BACKLIGHT_MAX)
	newval = X1600_BACKLIGHT_MAX;

      logdebug("LCD stepping +%d -> %d\n", lcd_x1600_cfg.step, newval);
    }
  else if (dir == STEP_DOWN)
    {
      newval = val - lcd_x1600_cfg.step;

      if (newval < X1600_BACKLIGHT_OFF)
	newval = X1600_BACKLIGHT_OFF;

      logdebug("LCD stepping -%d -> %d\n", lcd_x1600_cfg.step, newval);
    }
  else
    return;

  x1600_backlight_set((unsigned char)newval);

  x1600_backlight_unmap();

  mbpdbus_send_lcd_backlight(newval, val, LCD_USER);

  lcd_bck_info.level = newval;
}