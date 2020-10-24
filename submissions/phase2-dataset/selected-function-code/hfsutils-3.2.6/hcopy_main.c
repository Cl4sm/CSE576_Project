int hcopy_main(int argc, char *argv[])
{
  int nargs, mode = 'a', result = 0;
  const char *target;
  int fargc;
  char **fargv;
  hfsvol *vol;
  int (*copy)(hfsvol *, int, char *[], const char *, int);

  while (1)
    {
      int opt;

      opt = getopt(argc, argv, "mbtra");
      if (opt == EOF)
	break;

      switch (opt)
	{
	case '?':
	  return usage();

	default:
	  mode = opt;
	}
    }

  nargs = argc - optind;

  if (nargs < 2)
    return usage();

  target = argv[argc - 1];

  if (strchr(target, ':') && target[0] != '.' && target[0] != '/')
    {
      vol = hfsutil_remount(hcwd_getvol(-1), HFS_MODE_ANY);
      if (vol == 0)
	return 1;

      copy  = do_copyin;
      fargc = nargs - 1;
      fargv = &argv[optind];
    }
  else
    {
      vol = hfsutil_remount(hcwd_getvol(-1), HFS_MODE_RDONLY);
      if (vol == 0)
	return 1;

      copy  = do_copyout;
      fargv = hfsutil_glob(vol, nargs - 1, &argv[optind], &fargc, &result);
    }

  if (result == 0)
    result = copy(vol, fargc, fargv, target, mode);

  hfsutil_unmount(vol, &result);

  if (fargv && fargv != &argv[optind])
    free(fargv);

  return result;
}
