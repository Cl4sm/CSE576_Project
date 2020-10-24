int
mbp_sysfs_backlight_probe(void)
{
  int drv;
  int ret;

  for (drv = SYSFS_DRIVER_NONE + 1; drv < SYSFS_DRIVER_MAX; drv++)
    {
      ret = sysfs_backlight_probe(drv);
      if (ret == 0)
	return 0;
    }

  /* Probe failed, wire up native driver instead */
  switch (mops->type)
    {
      case MACHINE_MACBOOKPRO_3:
      case MACHINE_MACBOOKPRO_4:
      case MACHINE_MACBOOKPRO_5:
      case MACHINE_MACBOOKPRO_6:
      case MACHINE_MACBOOK_5:
      case MACHINE_MACBOOK_6:
      case MACHINE_MACBOOKAIR_2:
      case MACHINE_MACBOOKAIR_3:
	logmsg(LOG_INFO, "sysfs backlight probe failed, falling back to nv8600mgt");

	ret = nv8600mgt_backlight_probe();
	if (ret == 0)
	  {
	    /* Wire up fallback native driver */
	    mops->lcd_backlight_step = nv8600mgt_backlight_step;
	    mops->lcd_backlight_toggle = nv8600mgt_backlight_toggle;
	  }
	return ret;

      default:
	logmsg(LOG_ERR, "sysfs backlight probe failed, no fallback for this machine");
	return -1;
    }

  return -1;
}