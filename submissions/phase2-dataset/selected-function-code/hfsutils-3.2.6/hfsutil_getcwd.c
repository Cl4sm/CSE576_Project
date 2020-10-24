char *hfsutil_getcwd(hfsvol *vol)
{
  char *path, name[HFS_MAX_FLEN + 1 + 1];
  long cwd;
  int pathlen;

  path    = malloc(1);
  path[0] = 0;
  pathlen = 0;
  cwd     = hfs_getcwd(vol);

  while (cwd != HFS_CNID_ROOTPAR)
    {
      char *new;
      int namelen, i;

      if (hfs_dirinfo(vol, &cwd, name) == -1)
	return 0;

      if (pathlen)
	strcat(name, ":");

      namelen = strlen(name);

      new = realloc(path, namelen + pathlen + 1);
      if (new == 0)
	{
	  free(path);
	  ERROR(ENOMEM, 0);
	  return 0;
	}

      if (pathlen == 0)
	new[0] = 0;

      path = new;

      /* push string down to make room for path prefix (memmove()-ish) */

      i = pathlen + 1;
      for (new = path + namelen + pathlen; i--; new--)
	*new = *(new - namelen);

      memcpy(path, name, namelen);

      pathlen += namelen;
    }

  return path;
}
