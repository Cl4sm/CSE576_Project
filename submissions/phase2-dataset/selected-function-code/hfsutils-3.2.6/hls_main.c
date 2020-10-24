int hls_main(int argc, char *argv[])
{
  hfsvol *vol;
  int fargc, i;
  char **fargv = 0;
  int result = 0;
  int flags, options, width;
  char *ptr;
  darray *dirs, *files;

  options = T_MOD | S_NAME;

  if (isatty(STDOUT_FILENO))
    {
      options |= F_MANY;
      flags    = HLS_QMARK_CTRL;
    }
  else
    {
      options |= F_ONE;
      flags    = 0;
    }

  if (strcmp(bargv0, "hdir") == 0)
    options = (options & ~F_MASK) | F_LONG;

  ptr   = getenv("COLUMNS");
  width = ptr ? atoi(ptr) : 80;

# ifdef TIOCGWINSZ
  {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != -1 &&
	ws.ws_col != 0)
      width = ws.ws_col;
  }
# endif

  while (1)
    {
      int opt;

      opt = getopt(argc, argv, "1abcdfilmqrstxw:CFNQRSU");
      if (opt == EOF)
	break;

      switch (opt)
	{
	case '?':
	  return usage();

	case '1':
	  options = (options & ~F_MASK) | F_ONE;
	  break;

	case 'a':
	  flags |= HLS_ALL_FILES;
	  break;

	case 'b':
	  flags |= HLS_ESCAPE;
	  flags &= ~HLS_QMARK_CTRL;
	  break;

	case 'c':
	  options = (options & ~(T_MASK | S_MASK)) | T_CREATE | S_TIME;
	  break;

	case 'd':
	  flags |= HLS_IMMEDIATE_DIRS;
	  break;

	case 'f':
	  flags |= HLS_ALL_FILES;
	  flags &= ~HLS_SIZE;
	  options &= ~S_MASK;
	  if ((options & F_MASK) == F_LONG)
	    options = (options & ~F_MASK) |
	      (isatty(STDOUT_FILENO) ? F_MANY : F_ONE);
	  break;

	case 'i':
	  flags |= HLS_CATIDS;
	  break;

	case 'l':
	  options = (options & ~F_MASK) | F_LONG;
	  break;

	case 'm':
	  options = (options & ~F_MASK) | F_COMMAS;
	  break;

	case 'q':
	  flags |= HLS_QMARK_CTRL;
	  flags &= ~HLS_ESCAPE;
	  break;

	case 'r':
	  flags |= HLS_REVERSE;
	  break;

	case 's':
	  flags |= HLS_SIZE;
	  break;

	case 't':
	  options = (options & ~S_MASK) | S_TIME;
	  break;

	case 'x':
	  options = (options & ~F_MASK) | F_HORIZ;
	  break;

	case 'w':
	  width = atoi(optarg);
	  break;

	case 'C':
	  options = (options & ~F_MASK) | F_MANY;
	  break;

	case 'F':
	  flags |= HLS_INDICATOR;
	  break;

	case 'N':
	  flags &= ~(HLS_ESCAPE | HLS_QMARK_CTRL);
	  break;

	case 'Q':
	  flags |= HLS_QUOTE | HLS_ESCAPE;
	  flags &= ~HLS_QMARK_CTRL;
	  break;

	case 'R':
	  flags |= HLS_RECURSIVE;
	  break;

	case 'S':
	  options = (options & ~S_MASK) | S_SIZE;
	  break;

	case 'U':
	  options &= ~S_MASK;
	  break;
	}
    }

  vol = hfsutil_remount(hcwd_getvol(-1), HFS_MODE_RDONLY);
  if (vol == 0)
    return 1;

  fargv = hfsutil_glob(vol, argc - optind, &argv[optind], &fargc, &result);

  dirs  = qnew();
  files = qnew();
  if (result == 0 && (dirs == 0 || files == 0))
    {
      fprintf(stderr, "%s: not enough memory\n", argv0);
      result = 1;
    }

  if (result == 0)
    {
      if (fargc == 0)
	{
	  if (queuepath(vol, ":", dirs, files, flags) == -1)
	    result = 1;
	}
      else
	{
	  for (i = 0; i < fargc; ++i)
	    {
	      if (queuepath(vol, fargv[i], dirs, files, flags) == -1)
		{
		  result = 1;
		  break;
		}
	    }
	}
    }

  if (result == 0 && process(vol, dirs, files, flags, options, width) == -1)
    result = 1;

  if (files)
    qfree(files);
  if (dirs)
    qfree(dirs);

  hfsutil_unmount(vol, &result);

  if (fargv)
    free(fargv);

  return result;
}
