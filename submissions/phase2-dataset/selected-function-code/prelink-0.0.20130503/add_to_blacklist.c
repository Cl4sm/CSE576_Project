int
add_to_blacklist (const char *name, int deref, int onefs)
{
  const char *canon_name;
  struct prelink_dir *path;
  size_t len;
  struct stat64 st;

  if (stat64 (name, &st) < 0)
    {
      if (implicit)
	return 0;
      error (0, errno, "Could not stat %s", name);
      return 1;
    }

  if (!S_ISDIR (st.st_mode))
    {
      struct prelink_entry *ent;

      ent = prelink_find_entry (name, &st, 1);
      if (ent == NULL)
	return 1;

      ent->type = ET_BAD;
      ent->u.explicit = 1;
      return 0;
    }

  canon_name = prelink_canonicalize (name, NULL);
  if (canon_name == NULL)
    {
      if (implicit)
	return 0;
      error (0, errno, "Could not canonicalize %s", name);
      return 1;
    }

  len = strlen (canon_name);
  path = malloc (sizeof (struct prelink_dir) + len + 1);
  if (path == NULL)
    {
      error (0, ENOMEM, "Could not record path %s", name);
      free ((char *) canon_name);
      return 1;
    }

  path->next = blacklist;
  path->flags = 0;
  if (! deref) path->flags |= FTW_PHYS;
  if (onefs) path->flags |= FTW_MOUNT;
  path->dev = 0;
  path->len = len;
  strcpy (path->dir, canon_name);
  free ((char *) canon_name);
  blacklist = path;
  return 0;
}
