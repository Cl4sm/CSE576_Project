void
gma950_backlight_toggle(int lvl)
{
  int val;
  int ret;

  if (lcd_gma950_cfg.on_batt == 0)
    return;

  ret = gma950_backlight_map();
  if (ret < 0)
    return;

  val = gma950_backlight_get();
  if (val != lcd_bck_info.level)
    {
      mbpdbus_send_lcd_backlight(val, lcd_bck_info.level, LCD_AUTO);
      lcd_bck_info.level = val;
    }

  if (lcd_bck_info.level == 0)
    {
      gma950_backlight_unmap();
      return;
    }

  switch (lvl)
    {
      case LCD_ON_AC_LEVEL:
	if (lcd_bck_info.level >= lcd_bck_info.ac_lvl)
	  break;

	logdebug("LCD switching to AC level\n");

	gma950_backlight_set(lcd_bck_info.ac_lvl);

	mbpdbus_send_lcd_backlight(lcd_bck_info.ac_lvl, lcd_bck_info.level, LCD_AUTO);

	lcd_bck_info.level = lcd_bck_info.ac_lvl;
	break;

      case LCD_ON_BATT_LEVEL:
	if (lcd_bck_info.level <= lcd_gma950_cfg.on_batt)
	  break;

	logdebug("LCD switching to battery level\n");

	lcd_bck_info.ac_lvl = lcd_bck_info.level;

	gma950_backlight_set(lcd_gma950_cfg.on_batt);

	mbpdbus_send_lcd_backlight(lcd_gma950_cfg.on_batt, lcd_bck_info.level, LCD_AUTO);

	lcd_bck_info.level = lcd_gma950_cfg.on_batt;
	break;
    }

  gma950_backlight_unmap();
}