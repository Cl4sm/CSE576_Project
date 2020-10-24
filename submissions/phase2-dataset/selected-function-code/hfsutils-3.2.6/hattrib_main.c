{
  const char *type = 0, *crea = 0;
  int invis = 0, lock = 0, bless = 0;
  hfsvol *vol;
  int fargc;
  char **fargv;
  int i, result = 0;

  for (i = 1; i < argc; ++i)
    {
      switch (argv[i][0])
	{
	case '-':
	  switch (argv[i][1])
	    {
	    case 't':
	      type = argv[++i];

	      if (type == 0)
		return usage();

	      if (strlen(type) != 4)
		{
		  fprintf(stderr, "%s: file type must be 4 characters\n",
			  argv0);
		  return 1;
		}
	      continue;

	    case 'c':
	      crea = argv[++i];

	      if (crea == 0)
		return usage();

	      if (strlen(crea) != 4)
		{
		  fprintf(stderr, "%s: file creator must be 4 characters\n",
			  argv0);
		  return 1;
		}
	      continue;

	    case 'i':
	      invis = -1;
	      continue;

	    case 'l':
	      lock = -1;
	      continue;

	    case 'b':
	      bless = 1;
	      continue;

	    default:
	      return usage();
	    }
	  break;

	case '+':
	  switch (argv[i][1])
	    {
	    case 'i':
	      invis = 1;
	      continue;

	    case 'l':
	      lock = 1;
	      continue;

	    default:
	      return usage();
	    }
	  break;
	}

      break;
    }

  if (argc - i == 0)
    return usage();

  if (i == 1)
    {
      fprintf(stderr, "%s: no attributes specified\n", argv0);
      return 1;
    }

  if (bless && (lock || invis || type || crea || argc - i > 1))
    return usage();

  vol = hfsutil_remount(hcwd_getvol(-1), HFS_MODE_ANY);
  if (vol == 0)
    return 1;

  fargv = hfsutil_glob(vol, argc - i, &argv[i], &fargc, &result);

  if (result == 0)
    {
      hfsdirent ent;

      if (bless)
	{
	  if (fargc != 1)
	    {
	      fprintf(stderr, "%s: %s: ambiguous path\n", argv0, argv[i]);
	      result = 1;
	    }
	  else
	    {
	      hfsvolent volent;

	      if (hfs_stat(vol, fargv[0], &ent) == -1 ||
		  hfs_vstat(vol, &volent) == -1)
		{
		  hfsutil_perrorp(fargv[0]);
		  result = 1;
		}
	      else
		{
		  volent.blessed = ent.cnid;

		  if (hfs_vsetattr(vol, &volent) == -1)
		    {
		      hfsutil_perrorp(fargv[0]);
		      result = 1;
		    }
		}
	    }
	}
      else
	{
	  for (i = 0; i < fargc; ++i)
	    {
	      if (hfs_stat(vol, fargv[i], &ent) == -1)
		{
		  hfsutil_perrorp(fargv[i]);
		  result = 1;
		}
	      else
		{
		  if (! (ent.flags & HFS_ISDIR))
		    {
		      if (type)
			memcpy(ent.u.file.type, type, 4);
		      if (crea)
			memcpy(ent.u.file.creator, crea, 4);
		    }

		  if (invis < 0)
		    ent.fdflags &= ~HFS_FNDR_ISINVISIBLE;
		  else if (invis > 0)
		    ent.fdflags |= HFS_FNDR_ISINVISIBLE;

		  if (lock < 0)
		    ent.flags &= ~HFS_ISLOCKED;
		  else if (lock > 0)
		    ent.flags |= HFS_ISLOCKED;

		  if (hfs_setattr(vol, fargv[i], &ent) == -1)
		    {
		      hfsutil_perrorp(fargv[i]);
		      result = 1;
		    }
		}
	    }
	}
    }

  hfsutil_unmount(vol, &result);

  if (fargv)
    free(fargv);

  return result;
}
