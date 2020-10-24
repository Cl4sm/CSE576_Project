void
kbd_backlight_init(void)
{
  int ret;

  if (kbd_cfg.auto_on)
    kbd_bck_info.inhibit = 0;
  else
    kbd_bck_info.inhibit = KBD_INHIBIT_CFG;

  kbd_bck_info.toggle_lvl = kbd_cfg.auto_lvl;

  kbd_bck_info.inhibit_lvl = 0;

  kbd_bck_info.auto_on = 0;

  if (!has_kbd_backlight()
      || (mops->type == MACHINE_POWERBOOK_58)
      || (mops->type == MACHINE_POWERBOOK_59))
    {
      /* Nothing to probe for the PMU05 machines */
      ret = 0;
    }
  else
    ret = kbd_probe_lmu();

  if (!has_kbd_backlight() || (ret < 0))
    {
      lmu_info.lmuaddr = 0;

      kbd_bck_info.r_sens = 0;
      kbd_bck_info.l_sens = 0;

      kbd_bck_info.level = 0;

      ambient_info.left = 0;
      ambient_info.right = 0;
      ambient_info.max = 0;

      return;
    }

  kbd_bck_info.level = kbd_backlight_get();

  if (kbd_bck_info.level < 0)
    kbd_bck_info.level = 0;

  kbd_bck_info.max = KBD_BACKLIGHT_MAX;

  ambient_init(&kbd_bck_info.r_sens, &kbd_bck_info.l_sens);

  kbd_auto_init();
}