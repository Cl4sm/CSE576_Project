void
gma950_backlight_step(int dir)
{
  int ret;

  unsigned int val;
  unsigned int newval = 0;

  ret = gma950_backlight_map();
  if (ret < 0)
    return;

  val = gma950_backlight_get();

  if (dir == STEP_UP)
    {
      newval = val + lcd_gma950_cfg.step;

      if (newval < GMA950_BACKLIGHT_MIN)
	newval = GMA950_BACKLIGHT_MIN;

      if (newval > GMA950_BACKLIGHT_MAX)
	newval = GMA950_BACKLIGHT_MAX;

      logdebug("LCD stepping +%d -> %d\n", lcd_gma950_cfg.step, newval);
    }
  else if (dir == STEP_DOWN)
    {
      /* val is unsigned */
      if (val > lcd_gma950_cfg.step)
	newval = val - lcd_gma950_cfg.step;

      if (newval < GMA950_BACKLIGHT_MIN)
	newval = 0x00;

      logdebug("LCD stepping -%d -> %d\n", lcd_gma950_cfg.step, newval);
    }
  else
    return;

  gma950_backlight_set(newval);

  gma950_backlight_unmap();

  mbpdbus_send_lcd_backlight(newval, val, LCD_USER);

  lcd_bck_info.level = newval;
}