void
sysfs_backlight_toggle(int lvl)
{
  int val;

  if (bck_driver == SYSFS_DRIVER_NONE)
    return;

  if (lcd_sysfs_cfg.on_batt == 0)
    return;

  val = sysfs_backlight_get();
  if (val != lcd_bck_info.level)
    {
      mbpdbus_send_lcd_backlight(val, lcd_bck_info.level, LCD_AUTO);
      lcd_bck_info.level = val;
    }

  if (lcd_bck_info.level == 0)
    return;

  switch (lvl)
    {
      case LCD_ON_AC_LEVEL:
	if (lcd_bck_info.level >= lcd_bck_info.ac_lvl)
	  break;

	logdebug("LCD switching to AC level\n");

	sysfs_backlight_set(lcd_bck_info.ac_lvl);

	mbpdbus_send_lcd_backlight(lcd_bck_info.ac_lvl, lcd_bck_info.level, LCD_AUTO);

	lcd_bck_info.level = lcd_bck_info.ac_lvl;
	break;

      case LCD_ON_BATT_LEVEL:
	if (lcd_bck_info.level <= lcd_sysfs_cfg.on_batt)
	  break;

	logdebug("LCD switching to battery level\n");

	lcd_bck_info.ac_lvl = lcd_bck_info.level;

	sysfs_backlight_set(lcd_sysfs_cfg.on_batt);

	mbpdbus_send_lcd_backlight(lcd_sysfs_cfg.on_batt, lcd_bck_info.level, LCD_AUTO);

	lcd_bck_info.level = lcd_sysfs_cfg.on_batt;
	break;
    }
}