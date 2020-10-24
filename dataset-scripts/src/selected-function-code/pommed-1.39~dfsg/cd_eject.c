void
cd_eject(void)
{
  char *eject_argv[3] = { "eject", eject_cfg.device, NULL };
  char *eject_envp[1] = { NULL };
  long max_fd;
  int fd;
  int ret;

  if (!eject_cfg.enabled)
    return;

  fd = open(eject_cfg.device, O_RDONLY | O_NONBLOCK);
  if (fd < 0)
    {
      logmsg(LOG_ERR, "Could not open CD/DVD device: %s", strerror(errno));

      return;
    }

  /* Check drive status */
  ret = ioctl(fd, CDROM_DRIVE_STATUS);
  close(fd);

  switch (ret)
    {
      case CDS_NO_INFO: /* fall through to CDS_DISC_OK */
	logmsg(LOG_INFO, "Driver does not support CDROM_DRIVE_STATUS, trying to eject anyway");

      case CDS_DISC_OK:
	break;

      case CDS_NO_DISC:
	logmsg(LOG_INFO, "No disc in CD/DVD drive");
	return;

      case CDS_DRIVE_NOT_READY:
	logmsg(LOG_INFO, "Drive not ready, please retry later");
	return;

      case CDS_TRAY_OPEN:
	logmsg(LOG_INFO, "Drive tray already open");
	return;

      default:
	logmsg(LOG_INFO, "CDROM_DRIVE_STATUS returned %d (%s)", ret, strerror(errno));
	return;
    }

  ret = fork();
  if (ret == 0) /* exec eject */
    {
      max_fd = sysconf(_SC_OPEN_MAX);

      if (max_fd > INT_MAX)
	max_fd = INT_MAX;

      for (fd = 3; fd < max_fd; fd++)
	close(fd);

      execve("/usr/bin/eject", eject_argv, eject_envp);

      logmsg(LOG_ERR, "Could not execute eject: %s", strerror(errno));
      exit(1);
    }
  else if (ret == -1)
    {
      logmsg(LOG_ERR, "Could not fork: %s", strerror(errno));
      return;
    }
  else
    {
      mbpdbus_send_cd_eject();

      waitpid(ret, &ret, 0);
      if ((WIFEXITED(ret) == 0) || (WEXITSTATUS(ret) != 0))
	{
	  logmsg(LOG_INFO, "eject failed");
	  return;
	}
    }
}