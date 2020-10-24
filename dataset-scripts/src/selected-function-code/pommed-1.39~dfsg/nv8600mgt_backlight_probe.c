int
nv8600mgt_backlight_probe(void)
{
  int ret;

  /* Determine backlight I/O port */
  switch (mops->type)
    {
      case MACHINE_MACBOOKPRO_3:
      case MACHINE_MACBOOKPRO_4:
	bl_port = 0xb2; /* 0xb2 - 0xb3 */
	break;

      case MACHINE_MACBOOKPRO_5:
      case MACHINE_MACBOOKPRO_6:
      case MACHINE_MACBOOK_5:
      case MACHINE_MACBOOK_6:
      case MACHINE_MACBOOKAIR_2:
      case MACHINE_MACBOOKAIR_3:
	bl_port = 0x52e; /* 0x52e - 0x52f */
	break;

      default:
	logmsg(LOG_ERR, "nv8600mgt LCD backlight support not supported on this hardware");
	return -1;
    }

  lcd_bck_info.max = NV8600MGT_BACKLIGHT_MAX;

  ret = iopl(3);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "iopl() failed: %s", strerror(errno));

      lcd_bck_info.level = 0;

      return -1;
    }

  nv8600mgt_inited = 1;

  /*
   * Set the initial backlight level
   * The value has been sanity checked already
   */
  if (lcd_nv8600mgt_cfg.init > -1)
    {
      nv8600mgt_backlight_set((unsigned char)lcd_nv8600mgt_cfg.init);
    }

  lcd_bck_info.level = nv8600mgt_backlight_get();
  lcd_bck_info.ac_lvl = lcd_bck_info.level;

  return 0;
}