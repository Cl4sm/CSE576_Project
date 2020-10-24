static void
config_print(void)
{
  printf("pommed configuration:\n");
  printf(" + General settings:\n");
  printf("    fnmode: %d\n", general_cfg.fnmode);
  printf(" + sysfs backlight control:\n");
  printf("    initial level: %d\n", lcd_sysfs_cfg.init);
  printf("    step: %d\n", lcd_sysfs_cfg.step);
  printf("    on_batt: %d\n", lcd_sysfs_cfg.on_batt);
#ifndef __powerpc__
  printf(" + ATI X1600 backlight control:\n");
  printf("    initial level: %d\n", lcd_x1600_cfg.init);
  printf("    step: %d\n", lcd_x1600_cfg.step);
  printf("    on_batt: %d\n", lcd_x1600_cfg.on_batt);
  printf(" + Intel GMA950 backlight control:\n");
  printf("    initial level: 0x%x\n", lcd_gma950_cfg.init);
  printf("    step: 0x%x\n", lcd_gma950_cfg.step);
  printf("    on_batt: 0x%x\n", lcd_gma950_cfg.on_batt);
  printf(" + nVidia GeForce 8600M GT backlight control:\n");
  printf("    initial level: %d\n", lcd_nv8600mgt_cfg.init);
  printf("    step: %d\n", lcd_nv8600mgt_cfg.step);
  printf("    on_batt: %d\n", lcd_nv8600mgt_cfg.on_batt);
#endif /* !__powerpc__ */
  printf(" + Audio volume control:\n");
  if (audio_cfg.disabled)
    printf("    disabled: yes\n");
  else
    {
      printf("    card: %s\n", audio_cfg.card);
      printf("    initial volume: %d%s\n", audio_cfg.init, (audio_cfg.init > -1) ? "%" : "");
      printf("    step: %d%%\n", audio_cfg.step);
      printf("    beep: %s\n", (audio_cfg.beep) ? "yes" : "no");
      printf("    volume element: %s\n", audio_cfg.vol);
      printf("    speaker element: %s\n", audio_cfg.spkr);
      printf("    headphones element: %s\n", audio_cfg.head);
    }
  printf(" + Keyboard backlight control:\n");
  printf("    default level: %d\n", kbd_cfg.auto_lvl);
  printf("    step: %d\n", kbd_cfg.step);
  printf("    auto on threshold: %d\n", kbd_cfg.on_thresh);
  printf("    auto off threshold: %d\n", kbd_cfg.off_thresh);
  printf("    auto enable: %s\n", (kbd_cfg.auto_on) ? "yes" : "no");
  printf("    idle timer: %d%s\n", kbd_cfg.idle, (kbd_cfg.idle > 0) ? "s" : "");
  printf("    idle level: %d\n", kbd_cfg.idle_lvl);
  printf(" + CD eject:\n");
  printf("    enabled: %s\n", (eject_cfg.enabled) ? "yes" : "no");
  printf("    device: %s\n", eject_cfg.device);
  printf(" + Beep:\n");
  printf("    enabled: %s\n", (beep_cfg.enabled) ? "yes" : "no");
  printf("    beepfile: %s\n", beep_cfg.beepfile);
#ifndef __powerpc__
  printf(" + Apple Remote IR Receiver:\n");
  printf("    enabled: %s\n", (appleir_cfg.enabled) ? "yes" : "no");
#endif /* !__powerpc__ */
}