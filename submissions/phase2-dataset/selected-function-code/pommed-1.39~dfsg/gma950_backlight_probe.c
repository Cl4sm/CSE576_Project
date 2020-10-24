int
gma950_backlight_probe(void)
{
  struct pci_access *pacc;
  struct pci_dev *dev;
  struct stat stbuf;

  int card;
  int ret;

  pacc = pci_alloc();
  if (pacc == NULL)
    {
      logmsg(LOG_ERR, "Could not allocate PCI structs");
      return -1;
    }

  pci_init(pacc);
  pci_scan_bus(pacc);

  card = 0;
  /* Iterate over all devices */
  for(dev = pacc->devices; dev; dev = dev->next)
    {
      pci_fill_info(dev, PCI_FILL_IDENT);
      /* GMA950 or GMA965 */
      if ((dev->vendor_id == PCI_ID_VENDOR_INTEL)
	  && ((dev->device_id == PCI_ID_PRODUCT_GMA950)
	      || (dev->device_id == PCI_ID_PRODUCT_GMA965)))
	{
	  card = dev->device_id;

	  ret = snprintf(sysfs_resource, sizeof(sysfs_resource),
			 "/sys/bus/pci/devices/%04x:%02x:%02x.%1x/resource0",
			 dev->domain, dev->bus, dev->dev, dev->func);

	  break;
	}
    }

  pci_cleanup(pacc);

  if (!dev)
    {
      logdebug("Failed to detect Intel GMA950 or GMA965, aborting...\n");
      return -1;
    }

  /* Check snprintf() return value */
  if (ret >= sizeof(sysfs_resource))
    {
      logmsg(LOG_ERR, "Could not build sysfs PCI resource path");
      return -1;
    }

  ret = stat(sysfs_resource, &stbuf);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not determine PCI resource length: %s", strerror(errno));
      return -1;
    }

  length = stbuf.st_size;

  logdebug("GMA950/GMA965 PCI resource: [%s], length %ldK\n", sysfs_resource, (length / 1024));

  ret = gma950_backlight_map();
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not map GMA950/GMA965 memory");
      return -1;
    }

  if (card == PCI_ID_PRODUCT_GMA950)
    {
      if (INREG(GMA950_CONTROL_REGISTER) & GMA950_LEGACY_MODE)
	{
	  logdebug("GMA950 is in legacy backlight control mode, unsupported\n");

	  gma950_backlight_unmap();
	  return -1;
	}
    }
  else if (card == PCI_ID_PRODUCT_GMA965)
    {
      if (INREG(GMA965_CONTROL_REGISTER) & GMA965_LEGACY_MODE)
	{
	  logdebug("GMA965 is in legacy backlight control mode, unsupported\n");

	  gma950_backlight_unmap();
	  return -1;
	}
    }

  /* Get the maximum backlight value */
  GMA950_BACKLIGHT_MAX = gma950_backlight_get_max();

  logdebug("Max LCD backlight value is 0x%x\n", GMA950_BACKLIGHT_MAX);

  /* Now, check the config and fix it if needed */
  gma950_backlight_fix_config();

  /* Set the initial backlight level */
  if (lcd_gma950_cfg.init > -1)
    gma950_backlight_set(lcd_gma950_cfg.init);

  lcd_bck_info.max = GMA950_BACKLIGHT_MAX;
  lcd_bck_info.level = gma950_backlight_get();
  lcd_bck_info.ac_lvl = lcd_bck_info.level;

  gma950_backlight_unmap();

  return 0;
}