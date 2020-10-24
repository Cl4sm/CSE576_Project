int
load_one_module(const char *name)
{
  dlink_node *ptr = NULL;
  char path[HYB_PATH_MAX + 1];
  struct stat statbuf;

  DLINK_FOREACH(ptr, modules_path.head)
  {
    const struct module_path *mpath = ptr->data;

    snprintf(path, sizeof(path), "%s/%s", mpath->path, name);

    if (!modules_valid_suffix(name))
      continue;

    if (strstr(path, "../") == NULL &&
        strstr(path, "/..") == NULL)
      if (!stat(path, &statbuf))
        if (S_ISREG(statbuf.st_mode))  /* Regular files only please */
          return load_a_module(path, 1);
  }

  sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                       "Cannot locate module %s", name);
  ilog(LOG_TYPE_IRCD, "Cannot locate module %s", name);
  return -1;
}
