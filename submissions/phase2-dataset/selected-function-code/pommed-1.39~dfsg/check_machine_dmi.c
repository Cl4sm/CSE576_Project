static machine_type
check_machine_dmi(void)
{
  int ret;

  int fd;
  char buf[32];
  int i;

  char *vendor_node[] =
    {
      "/sys/class/dmi/id/sys_vendor",
      "/sys/class/dmi/id/board_vendor",
      "/sys/class/dmi/id/chassis_vendor",
      "/sys/class/dmi/id/bios_vendor"
    };

  /* Check vendor name */
  for (i = 0; i < sizeof(vendor_node) / sizeof(vendor_node[0]); i++)
    {
      fd = open(vendor_node[i], O_RDONLY);
      if (fd > 0)
	break;

      logmsg(LOG_INFO, "Could not open %s: %s", vendor_node[i], strerror(errno));
    }

  if (fd < 0)
    {
      logmsg(LOG_ERR, "Could not determine vendor name");

      return MACHINE_ERROR;
    }

  memset(buf, 0, sizeof(buf));

  ret = read(fd, buf, sizeof(buf) - 1);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not read from %s: %s", vendor_node[i], strerror(errno));

      close(fd);
      return MACHINE_ERROR;
    }

  close(fd);

  if (buf[ret - 1] == '\n')
    buf[ret - 1] = '\0';

  logdebug("DMI vendor name: [%s]\n", buf);

  if ((strcmp(buf, "Apple Computer, Inc.") != 0)
      && (strcmp(buf, "Apple Inc.") != 0))
    return MACHINE_UNKNOWN;

  /* Check product name */
  fd = open("/sys/class/dmi/id/product_name", O_RDONLY);
  if (fd < 0)
    {
      logmsg(LOG_INFO, "Could not open /sys/class/dmi/id/product_name: %s", strerror(errno));

      return MACHINE_MAC_UNKNOWN;
    }

  memset(buf, 0, sizeof(buf));

  ret = read(fd, buf, sizeof(buf) - 1);
  if (ret < 0)
    {
      logmsg(LOG_ERR, "Could not read from /sys/class/dmi/id/product_name: %s", strerror(errno));

      close(fd);
      return MACHINE_MAC_UNKNOWN;
    }

  close(fd);

  if (buf[ret - 1] == '\n')
    buf[ret - 1] = '\0';

  logdebug("DMI product name: [%s]\n", buf);

  ret = MACHINE_MAC_UNKNOWN;

  /* Core Duo MacBook Pro 15" (January 2006) & 17" (April 2006) */
  if ((strcmp(buf, "MacBookPro1,1") == 0) || (strcmp(buf, "MacBookPro1,2") == 0))
    ret = MACHINE_MACBOOKPRO_1;
  /* Core2 Duo MacBook Pro 17" & 15" (October 2006) */
  else if ((strcmp(buf, "MacBookPro2,1") == 0) || (strcmp(buf, "MacBookPro2,2") == 0))
    ret = MACHINE_MACBOOKPRO_2;
  /* Core2 Duo MacBook Pro 15" & 17" (June 2007) */
  else if (strcmp(buf, "MacBookPro3,1") == 0)
    ret = MACHINE_MACBOOKPRO_3;
  /* Core2 Duo MacBook Pro 15" & 17" (February 2008) */
  else if (strcmp(buf, "MacBookPro4,1") == 0)
    ret = MACHINE_MACBOOKPRO_4;
  /* Core2 Duo MacBook Pro 15" & 17" (October 2008)
   * MacBook Pro 17" (June 2009)
   * MacBook Pro 13" (June 2009) */
  else if ((strcmp(buf, "MacBookPro5,1") == 0)
	   || (strcmp(buf, "MacBookPro5,2") == 0)
	   || (strcmp(buf, "MacBookPro5,3") == 0)
	   || (strcmp(buf, "MacBookPro5,4") == 0)
	   || (strcmp(buf, "MacBookPro5,5") == 0))
    ret = MACHINE_MACBOOKPRO_5;
  /* Core i5/i7 MacBook Pro 15" & 17" (April 2010) */
  else if ((strcmp(buf, "MacBookPro6,1") == 0)
	   || (strcmp(buf, "MacBookPro6,2") == 0))
    ret = MACHINE_MACBOOKPRO_6;
  /* Core2 Duo MacBook Pro 13" (April 2010) */
  else if (strcmp(buf, "MacBookPro7,1") == 0)
    ret = MACHINE_MACBOOKPRO_7;
  /* MacBook Pro 13" (Early 2011)
   * MacBook Pro 15" (Early 2011)
   * MacBook Pro 17" (Early 2011)
   */
  else if ((strcmp(buf, "MacBookPro8,1") == 0)
	   || (strcmp(buf, "MacBookPro8,2") == 0)
	   || (strcmp(buf, "MacBookPro8,3") == 0))
    ret = MACHINE_MACBOOKPRO_8;
  /* MacBook Pro 13" (June 2012)
   * MacBook Pro 15" (June 2012)
   */
  else if ((strcmp(buf, "MacBookPro9,1") == 0)
	   || (strcmp(buf, "MacBookPro9,2") == 0))
    ret = MACHINE_MACBOOKPRO_9;
  /* Core Duo MacBook (May 2006) */
  else if (strcmp(buf, "MacBook1,1") == 0)
    ret = MACHINE_MACBOOK_1;
  /* Core2 Duo MacBook (November 2006) */
  else if (strcmp(buf, "MacBook2,1") == 0)
    ret = MACHINE_MACBOOK_2;
  /* Core2 Duo Santa Rosa MacBook (November 2007) */
  else if (strcmp(buf, "MacBook3,1") == 0)
    ret = MACHINE_MACBOOK_3;
  /* Core2 Duo MacBook (February 2008) */
  else if (strcmp(buf, "MacBook4,1") == 0)
    ret = MACHINE_MACBOOK_4;
  /* Core2 Duo MacBook (October 2008) (5,2 white MacBook) */
  else if ((strcmp(buf, "MacBook5,1") == 0) || (strcmp(buf, "MacBook5,2") == 0))
    ret = MACHINE_MACBOOK_5;
  /* Core2 Duo MacBook (October 2009) */
  else if (strcmp(buf, "MacBook6,1") == 0)
    ret = MACHINE_MACBOOK_6;
  /* Core2 Duo MacBook (April 2010) */
  else if (strcmp(buf, "MacBook7,1") == 0)
    ret = MACHINE_MACBOOK_7;
  /* MacBook Air (January 2008) */
  else if (strcmp(buf, "MacBookAir1,1") == 0)
    ret = MACHINE_MACBOOKAIR_1;
  /* MacBook Air (October 2008) */
  else if (strcmp(buf, "MacBookAir2,1") == 0)
    ret = MACHINE_MACBOOKAIR_2;
  /* MacBook Air 11" & 13" (October 2010) */
  else if ((strcmp(buf, "MacBookAir3,1") == 0) || (strcmp(buf, "MacBookAir3,2") == 0))
    ret = MACHINE_MACBOOKAIR_3;
  else
    logmsg(LOG_ERR, "Unknown Apple machine: %s", buf);

  if (ret != MACHINE_MAC_UNKNOWN)
    logmsg(LOG_INFO, "DMI machine check: running on a %s", buf);

  return ret;
}