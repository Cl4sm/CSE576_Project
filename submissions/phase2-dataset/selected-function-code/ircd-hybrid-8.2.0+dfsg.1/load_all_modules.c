load_all_modules(int warn)
{
  DIR *system_module_dir = NULL;
  struct dirent *ldirent = NULL;
  char module_fq_name[HYB_PATH_MAX + 1];

  if ((system_module_dir = opendir(AUTOMODPATH)) == NULL)
  {
    ilog(LOG_TYPE_IRCD, "Could not load modules from %s: %s",
         AUTOMODPATH, strerror(errno));
    return;
  }

  while ((ldirent = readdir(system_module_dir)))
  {
    if (modules_valid_suffix(ldirent->d_name))
    {
       snprintf(module_fq_name, sizeof(module_fq_name), "%s/%s",
                AUTOMODPATH, ldirent->d_name);
       load_a_module(module_fq_name, warn);
    }
  }

  closedir(system_module_dir);
}
