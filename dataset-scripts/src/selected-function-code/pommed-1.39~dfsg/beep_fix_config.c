void
beep_fix_config(void)
{
  if (beep_cfg.enabled == 0)
    return;

  if (beep_cfg.beepfile == NULL)
    beep_cfg.beepfile = strdup(BEEP_DEFAULT_FILE);

  if (access(beep_cfg.beepfile, R_OK) != 0)
    {
      logmsg(LOG_WARNING, "beep: cannot access WAV file %s: %s", beep_cfg.beepfile, strerror(errno));

      if (access(BEEP_DEFAULT_FILE, R_OK) == 0)
	{
	  logmsg(LOG_WARNING, "beep: falling back to default file %s", BEEP_DEFAULT_FILE);

	  free(beep_cfg.beepfile);
	  beep_cfg.beepfile = strdup(BEEP_DEFAULT_FILE);
	}
      else
	{
	  logmsg(LOG_ERR, "beep: cannot access default file %s: %s", BEEP_DEFAULT_FILE, strerror(errno));
	  logmsg(LOG_ERR, "beep: disabling beeper");

	  beep_cfg.enabled = 0;
	}
    }
}