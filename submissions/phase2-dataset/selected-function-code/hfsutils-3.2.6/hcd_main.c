{
  mountent *ment;
  hfsvol *vol;
  hfsvolent vent;
  char *path, root[HFS_MAX_VLEN + 1 + 1];
  int fargc;
  char **fargv = 0;
  int result = 0;

  if (argc > 2)
    {
      fprintf(stderr, "Usage: %s [hfs-path]\n", argv0);
      return 1;
    }

  vol = hfsutil_remount(ment = hcwd_getvol(-1), HFS_MODE_RDONLY);
  if (vol == 0)
    return 1;

  if (argc == 2)
    {
      fargv = hfsutil_glob(vol, 1, &argv[1], &fargc, &result);
      if (result == 0)
	{
	  if (fargc != 1)
	    {
	      fprintf(stderr, "%s: %s: ambiguous path\n", argv0, argv[1]);
	      result = 1;
	    }
	  else
	    path = fargv[0];
	}
    }
  else
    {
      hfs_vstat(vol, &vent);

      strcpy(root, vent.name);
      strcat(root, ":");
      path = root;
    }

  if (result == 0)
    {
      if (hfs_chdir(vol, path) == -1)
	{
	  hfsutil_perrorp(path);
	  result = 1;
	}
      else
	{
	  path = hfsutil_getcwd(vol);
	  if (path == 0)
	    {
	      hfsutil_perror("Can't get new HFS directory path");
	      result = 1;
	    }

	  if (result == 0 && hcwd_setcwd(ment, path) == -1)
	    {
	      perror("Can't set new HFS directory");
	      result = 1;
	    }

	  if (path)
	    free(path);
	}
    }

  hfsutil_unmount(vol, &result);

  if (fargv)
    free(fargv);

  return result;
}
