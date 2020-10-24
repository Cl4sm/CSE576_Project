static int
sysfs_check_ac_state(void)
{
  FILE *fp;
  char ac_state;

  fp = fopen(SYSFS_POWER_AC_STATE, "r");
  if (fp == NULL)
    return AC_STATE_ERROR;

  fread(&ac_state, 1, 1, fp);

  if (ferror(fp) != 0)
    {
      logdebug("power: Error reading sysfs AC state: %s\n", strerror(errno));
      return AC_STATE_ERROR;
    }

  fclose(fp);

  if (ac_state == '1')
    return AC_STATE_ONLINE;

  if (ac_state == '0')
    return AC_STATE_OFFLINE;

  return AC_STATE_UNKNOWN;
}