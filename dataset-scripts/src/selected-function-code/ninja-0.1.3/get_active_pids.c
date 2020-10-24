int* get_active_pids(void)
{
  DIR *dp;
  struct dirent *d;
  static int active_pids[PID_MAX];
  int pidnr;
  int i;

  if ((dp = opendir("/proc")) == NULL)
    return NULL;

  for (i = 0; i < PID_MAX; i++)
    active_pids[i] = 0;

  while ((d = readdir(dp)) != NULL) {

    pidnr = strtol(d->d_name, NULL, 0);

    if (pidnr > 0)
      active_pids[pidnr] = 1;

  }

  closedir(dp);

  return active_pids;
}