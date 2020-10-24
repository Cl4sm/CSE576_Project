static int
sysfs_backlight_probe(int driver)
{
  if (access(brightness[driver], W_OK) != 0)
    {
      logdebug("Failed to access brightness node: %s\n", strerror(errno));
      return -1;
    }

  if (access(actual_brightness[driver], R_OK) != 0)
    {
      logdebug("Failed to access actual_brightness node: %s\n", strerror(errno));
      return -1;
    }

  if (access(max_brightness[driver], R_OK) != 0)
    {
      logdebug("Failed to access max_brightness node: %s\n", strerror(errno));
      return -1;
    }

  bck_driver = driver;

  lcd_bck_info.max = sysfs_backlight_get_max();

  /* Now we can fix the config */
  sysfs_backlight_fix_config();

  /*
   * Set the initial backlight level
   * The value has been sanity checked already
   */
  if (lcd_sysfs_cfg.init > -1)
    {
      sysfs_backlight_set(lcd_sysfs_cfg.init);
    }

  lcd_bck_info.level = sysfs_backlight_get();
  lcd_bck_info.ac_lvl = lcd_bck_info.level;

  return 0;
}