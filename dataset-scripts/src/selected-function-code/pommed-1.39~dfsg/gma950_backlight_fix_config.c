static void
gma950_backlight_fix_config(void)
{
  if (lcd_gma950_cfg.init < 0)
    lcd_gma950_cfg.init = -1;

  if (lcd_gma950_cfg.init > GMA950_BACKLIGHT_MAX)
    lcd_gma950_cfg.init = GMA950_BACKLIGHT_MAX;

  if ((lcd_gma950_cfg.init < GMA950_BACKLIGHT_MIN)
      && (lcd_gma950_cfg.init > 0))
    lcd_gma950_cfg.init = 0x00;

  if (lcd_gma950_cfg.step < 1)
    lcd_gma950_cfg.step = 1;

  if (lcd_gma950_cfg.step > 0x20)
    lcd_gma950_cfg.step = 0x20;

  if ((lcd_gma950_cfg.on_batt > GMA950_BACKLIGHT_MAX)
      || (lcd_gma950_cfg.on_batt < GMA950_BACKLIGHT_MIN))
    lcd_gma950_cfg.on_batt = 0;
}