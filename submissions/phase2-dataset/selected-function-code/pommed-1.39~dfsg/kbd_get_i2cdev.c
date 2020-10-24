static int
kbd_get_i2cdev(void)
{
  char buf[PATH_MAX];
  int i2c_bus;
  int ret;

  FILE *fp;

  /* All the 256 minors (major 89) are reserved for i2c adapters */
  for (i2c_bus = 0; i2c_bus < 256; i2c_bus++)
    {
      ret = snprintf(buf, PATH_MAX - 1, "%s/i2c-%d/name", SYSFS_I2C_BASE, i2c_bus);
      if ((ret < 0) || (ret >= (PATH_MAX - 1)))
	{
	  logmsg(LOG_WARNING, "Error: i2c device probe: device path too long");

	  i2c_bus = 256;
	  break;
	}

      fp = fopen(buf, "r");
      if (fp == NULL)
	{
	  if (errno != ENOENT)
	    {
	      logmsg(LOG_ERR, "Error: i2c device probe: cannot open %s: %s", buf, strerror(errno));
	      continue;
	    }
	  else
	    {
	      logmsg(LOG_ERR, "Error: i2c device probe: i2c device not found, is i2c-dev loaded?");

	      i2c_bus = 256;
	      break;
	    }
	}

      ret = fread(buf, 1, PATH_MAX - 1, fp);
      fclose(fp);

      if (ret < 1)
	continue;

      buf[ret - 1] = '\0';

      logdebug("Found i2c adapter [%s]\n", buf);

      if (ret < strlen(I2C_ADAPTER_NAME))
	continue;

      if (strncmp(buf, I2C_ADAPTER_NAME, strlen(I2C_ADAPTER_NAME)) == 0)
	{
	  logmsg(LOG_INFO, "Found %s i2c adapter at i2c-%d", I2C_ADAPTER_NAME, i2c_bus);
	  break;
	}
    }

  if (i2c_bus > 255)
    return -1;

  ret = snprintf(lmu_info.i2cdev, sizeof(lmu_info.i2cdev) - 1, "/dev/i2c-%d", i2c_bus);
  if ((ret < 0) || (ret >= (sizeof(lmu_info.i2cdev) - 1)))
    {
      logmsg(LOG_WARNING, "Error: i2c device path too long");

      return -1;
    }

  return 0;
}