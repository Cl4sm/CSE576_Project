int hcwd_mounted(const char *vname, long vcrdate, const char *path, int partno)
{
  mountent *entry, new;

  for (entry = mounts; entry < mounts + nmounts; ++entry)
    {
      if (strcmp(entry->path, path) == 0 &&
	  entry->partno == partno)
	{
	  /* update entry */

	  if (entry->cwd)
	    free(entry->cwd);

	  strcpy(entry->vname, vname);
	  entry->vcrdate = vcrdate;
	  entry->cwd     = strdup(":");
	  if (entry->cwd == 0)
	    return -1;

	  curvol = entry - mounts;
	  dirty  = 1;

	  return 0;
	}
    }

  strcpy(new.vname, vname);
  new.vcrdate = vcrdate;
  new.path    = strdup(path);
  new.partno  = partno;
  new.cwd     = strdup(":");

  if (new.path  == 0 ||
      new.cwd   == 0)
    return -1;

  if (addent(&new) == -1)
    return -1;

  curvol = nmounts - 1;

  return 0;
}
