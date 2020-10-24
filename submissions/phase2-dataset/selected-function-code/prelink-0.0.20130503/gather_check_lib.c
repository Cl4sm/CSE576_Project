gather_check_lib (void **p, void *info)
{
  struct prelink_entry *e = * (struct prelink_entry **) p;

  if (e->type != ET_DYN)
    return 1;

  if (! e->u.explicit)
    {
      struct prelink_dir *dir;
      const char *name;
      size_t len;

      name = strrchr (e->canon_filename, '/');
      if (!name)
	name = e->canon_filename;
      len = name - e->canon_filename;

      for (dir = blacklist; dir; dir = dir->next)
	if (((dir->flags != FTW_CHDIR && len >= dir->len)
	     || (dir->flags == FTW_CHDIR && len == dir->len))
	    && strncmp (dir->dir, e->canon_filename, dir->len) == 0)
	  {
	    if (dir->flags == FTW_CHDIR)
	      break;
	    if ((dir->flags & FTW_MOUNT) && dir->dev != e->dev)
	      continue;
	    break;
	  }

      if (dir != NULL)
	{
	  error (0, 0, "%s is present in a blacklisted directory %s",
		 e->canon_filename, dir->dir);
	  e->type = ET_BAD;
	  return 1;
	}

      for (dir = dirs; dir; dir = dir->next)
	if (((dir->flags != FTW_CHDIR && len >= dir->len)
	     || (dir->flags == FTW_CHDIR && len == dir->len))
	    && strncmp (dir->dir, e->canon_filename, dir->len) == 0)
	  {
	    if (dir->flags == FTW_CHDIR)
	      break;
	    if ((dir->flags & FTW_MOUNT) && dir->dev != e->dev)
	      continue;
	    break;
	  }

      if (dir == NULL)
	{
	  error (0, 0, "%s is not present in any config file directories, nor was specified on command line",
		 e->canon_filename);
	  e->type = ET_BAD;
	  return 1;
	}
    }

  return 1;
}
