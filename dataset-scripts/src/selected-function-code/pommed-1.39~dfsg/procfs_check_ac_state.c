int
procfs_check_ac_state(void)
{
  FILE *fp;
  char buf[128];
  char *ac_state;
  int ret;

  fp = fopen(PROC_PMU_AC_STATE_FILE, "r");
  if (fp == NULL)
    return AC_STATE_ERROR;

  ret = fread(buf, 1, 127, fp);

  if (ferror(fp) != 0)
    {
      logdebug("pmu: Error reading AC state: %s\n", strerror(errno));
      return AC_STATE_ERROR;
    }

  if (feof(fp) == 0)
    {
      logdebug("pmu: Error reading AC state: buffer too small\n");
      return AC_STATE_ERROR;
    }

  fclose(fp);

  buf[ret] = '\0';

  ac_state = strstr(buf, PROC_PMU_AC_STATE);
  if (ac_state == NULL)
    return AC_STATE_ERROR;

  ac_state = strchr(ac_state, '\n');
  if ((ac_state == NULL) || (ac_state == buf))
    return AC_STATE_ERROR;

  if (ac_state[-1] == PROC_PMU_AC_ONLINE)
    return AC_STATE_ONLINE;

  if (ac_state[-1] == PROC_PMU_AC_OFFLINE)
    return AC_STATE_OFFLINE;

  return AC_STATE_UNKNOWN;
}