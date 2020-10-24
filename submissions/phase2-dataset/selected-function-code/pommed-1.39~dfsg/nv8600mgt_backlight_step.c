void
nv8600mgt_backlight_step(int dir)
{
  int val;
  int newval;

  if (nv8600mgt_inited == 0)
    return;

  val = nv8600mgt_backlight_get();

  if (dir == STEP_UP)
    {
      newval = val + lcd_nv8600mgt_cfg.step;

      if (newval > NV8600MGT_BACKLIGHT_MAX)
	newval = NV8600MGT_BACKLIGHT_MAX;

      logdebug("LCD stepping +%d -> %d\n", lcd_nv8600mgt_cfg.step, newval);
    }
  else if (dir == STEP_DOWN)
    {
      newval = val - lcd_nv8600mgt_cfg.step;

      if (newval < NV8600MGT_BACKLIGHT_OFF)
	newval = NV8600MGT_BACKLIGHT_OFF;

      logdebug("LCD stepping -%d -> %d\n", lcd_nv8600mgt_cfg.step, newval);
    }
  else
    return;

  nv8600mgt_backlight_set((unsigned char)newval);

  mbpdbus_send_lcd_backlight(newval, val, LCD_USER);

  lcd_bck_info.level = newval;
}