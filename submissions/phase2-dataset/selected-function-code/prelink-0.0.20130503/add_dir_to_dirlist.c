static int
add_dir_to_dirlist (const char *name, dev_t dev, int flags)
{
  const char *canon_name;
  struct prelink_dir *dir;
  size_t len;

  canon_name = prelink_canonicalize (name, NULL);
  if (canon_name == NULL)
    {
      if (! all && implicit)
	return 0;
      error (0, errno, "Could not record directory %s", name);
      return 1;
    }

  len = strlen (canon_name);

  for (dir = blacklist; dir; dir = dir->next)
    if (((dir->flags != FTW_CHDIR && len >= dir->len)
	 || (dir->flags == FTW_CHDIR && len == dir->len))
	&& strncmp (dir->dir, canon_name, dir->len) == 0)
      {
	if (dir->flags == FTW_CHDIR)
	  break;
	if ((dir->flags & FTW_MOUNT) && dir->dev != dev)
	  continue;
	break;
      }

  if (dir != NULL)
    {
      free ((char *) canon_name);
      return 2;
    }

  dir = malloc (sizeof (struct prelink_dir) + len + 1);
  if (dir == NULL)
    {
      error (0, ENOMEM, "Could not record directory %s", name);
      free ((char *) canon_name);
      return 1;
    }

  dir->next = dirs;
  dir->flags = flags;
  dir->dev = dev;
  dir->len = len;
  strcpy (dir->dir, canon_name);
  free ((char *) canon_name);
  dirs = dir;
  return 0;
}
