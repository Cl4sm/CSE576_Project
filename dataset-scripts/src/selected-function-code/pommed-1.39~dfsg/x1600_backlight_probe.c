int
x1600_backlight_probe(void)
{
  struct pci_access *pacc;
  struct pci_dev *dev;
  struct stat stbuf;

  int ret;

  pacc = pci_alloc();
  if (pacc == NULL)
    {
      logmsg(LOG_ERR, "Could not allocate PCI structs");
      return -1;
    }

  pci_init(pacc);
  pci_scan_bus(pacc);

  /* Iterate over all devices */
  for(dev = pacc->devices; dev; dev = dev->next)
    {
      pci_fill_info(dev, PCI_FILL_IDENT);
      /* ATI X1600 */
      if ((dev->vendor_id == PCI_ID_VENDOR_ATI)
	  && (dev->device_id == PCI_ID_PRODUCT_X1600))
	{
	  ret = snprintf(sysfs_resource, sizeof(sysfs_resource),
			 "/sys/bus/pci/devices/%04x:%02x:%02x.%1x/resource2",
			 dev->domain, dev->bus, dev->dev, dev->func);

	  break;
	}
    }

  pci_cleanup(pacc);

  if (!dev)
    {
      logdebug("Failed to detect ATI X1600, aborting...\n");
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

  logdebug("ATI X1600 PCI resource: [%s], length %ldK\n", sysfs_resource, (length / 1024));

  lcd_bck_info.max = X1600_BACKLIGHT_MAX;

  ret = x1600_backlight_map();
  if (ret < 0)
    {
      lcd_bck_info.level = 0;

      return 0;
    }

  /*
   * Set the initial backlight level
   * The value has been sanity checked already
   */
  if (lcd_x1600_cfg.init > -1)
    {
      x1600_backlight_set((unsigned char)lcd_x1600_cfg.init);
    }

  lcd_bck_info.level = x1600_backlight_get();
  lcd_bck_info.ac_lvl = lcd_bck_info.level;

  x1600_backlight_unmap();

  return 0;
}