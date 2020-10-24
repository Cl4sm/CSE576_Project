int hcwd_setcwd(mountent *ent, const char *newcwd)
{
  const char *path;
  char *cwd;

  path = newcwd;
  while (*path && *path != ':')
    ++path;

  memcpy(ent->vname, newcwd, path - newcwd);
  ent->vname[path - newcwd] = 0;

  cwd = (*path == 0) ? strdup(":") : strdup(path);
  if (cwd == 0)
    return -1;

  if (ent->cwd)
    free(ent->cwd);

  ent->cwd = cwd;
  dirty    = 1;

  return 0;
}
