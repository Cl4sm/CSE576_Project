void
ambient_init(int *r, int *l)
{
  char devpath[PATH_MAX];
  char devname[9];
  char *p;
  DIR *pdev;
  struct dirent *pdevent;
  int fd;
  int ret;

  smcpath = NULL;

  /* Probe for the applesmc sysfs path */
  pdev = opendir(HWMON_SYSFS_BASE);
  if (pdev != NULL)
    {
      while ((pdevent = readdir(pdev)))
	{
	  if (pdevent->d_name[0] == '.')
	    continue;

	  ret = snprintf(devpath, sizeof(devpath), HWMON_SYSFS_BASE "/%s/device/name", pdevent->d_name);
	  if ((ret < 0) || (ret >= sizeof(devpath)))
	    {
	      logmsg(LOG_WARNING, "Failed to build hwmon probe path");
	      continue;
	    }

	  fd = open(devpath, O_RDONLY);
	  if (fd < 0)
	    {
	      logmsg(LOG_ERR, "Could not open %s: %s", devpath, strerror(errno));
	      continue;
	    }

	  memset(devname, 0, sizeof(devname));
	  ret = read(fd, devname, sizeof(devname) - 1);
	  close(fd);

	  if (ret != (sizeof(devname) - 1))
	    continue;

	  if (strcmp(devname, "applesmc") == 0)
	    {
	      p = strrchr(devpath, '/');
	      *p = '\0';

	      logmsg(LOG_INFO, "Found applesmc at %s", devpath);

	      smcpath = realpath(devpath, NULL);
	      if (!smcpath)
		{
		  logmsg(LOG_ERR, "Could not dereference applesmc device path: %s\n", strerror(errno));
		  break;
		}

	      logmsg(LOG_INFO, "Dereferenced applesmc to %s", smcpath);

	      smcpath = realloc(smcpath, strlen(smcpath) + 1 + 6);
	      if (!smcpath)
		break;

	      strcat(smcpath, "/light");

	      break;
	    }
	}

      closedir(pdev);
    }

  ambient_get(r, l);

  ambient_info.max = KBD_AMBIENT_MAX;
  ambient_info.left = *l;
  ambient_info.right = *r;
}