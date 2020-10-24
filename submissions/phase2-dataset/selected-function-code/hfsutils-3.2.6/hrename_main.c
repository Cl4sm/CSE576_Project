int hrename_main(int argc, char *argv[])
{
  mountent *ment;
  hfsvol *vol;
  int fargc;
  char **fargv;
  int result = 0;

  if (argc < 3)
    {
      fprintf(stderr, "Usage: %s hfs-src-path [...] hfs-target-path\n", argv0);
      return 1;
    }

  vol = hfsutil_remount(ment = hcwd_getvol(-1), HFS_MODE_ANY);
  if (vol == 0)
    return 1;

  fargv = hfsutil_glob(vol, argc - 2, &argv[1], &fargc, &result);

  if (result == 0)
    result = do_rename(vol, fargc, fargv, argv[argc - 1]);

  if (result == 0)
    {
      char *path;

      path = hfsutil_getcwd(vol);
      if (path == 0)
	{
	  hfsutil_perror("Can't get current HFS directory path");
	  result = 1;
	}
      else if (hcwd_setcwd(ment, path) == -1)
	{
	  perror("Can't set current HFS directory");
	  result = 1;
	}

      if (path)
	free(path);
    }

  hfsutil_unmount(vol, &result);

  if (fargv)
    free(fargv);

  return result;
}
