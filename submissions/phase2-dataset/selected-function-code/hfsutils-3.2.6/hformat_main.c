{
  const char *vname;
  char *path = 0;
  hfsvol *vol;
  hfsvolent ent;
  int nparts, partno, options = 0, result = 0;

  vname = "Untitled";

  while (1)
    {
      int opt;

      opt = getopt(argc, argv, "fl:");
      if (opt == EOF)
	break;

      switch (opt)
	{
	case '?':
	  usage();
	  goto fail;

	case 'f':
	  options |= O_FORCE;
	  break;

	case 'l':
	  vname = optarg;
	  break;
	}
    }

  if (argc - optind < 1 || argc - optind > 2)
    {
      usage();
      goto fail;
    }

  path = hfsutil_abspath(argv[optind]);
  if (path == 0)
    {
      fprintf(stderr, "%s: not enough memory\n", argv0);
      goto fail;
    }

  suid_enable();
  nparts = hfs_nparts(path);
  suid_disable();

  if (argc - optind == 2)
    {
      partno = atoi(argv[optind + 1]);

      if (nparts != -1 && partno == 0)
	{
	  if (options & O_FORCE)
	    {
	      fprintf(stderr,
		      "%s: warning: erasing partition information\n", argv0);
	    }
	  else
	    {
	      fprintf(stderr, "%s: medium is partitioned; "
		      "select partition > 0 or use -f\n", argv0);
	      goto fail;
	    }
	}
    }
  else
    {
      if (nparts > 1)
	{
	  fprintf(stderr,
		  "%s: must specify partition number (%d available)\n",
		  argv0, nparts);
	  goto fail;
	}
      else if (nparts == -1)
	partno = 0;
      else
	partno = 1;
    }

  vol = do_format(path, partno, 0, vname);
  if (vol == 0)
    {
      hfsutil_perror(path);
      goto fail;
    }

  hfs_vstat(vol, &ent);
  hfsutil_pinfo(&ent);

  if (hcwd_mounted(ent.name, ent.crdate, path, partno) == -1)
    {
      perror("Failed to record mount");
      result = 1;
    }

  hfsutil_unmount(vol, &result);

  free(path);

  return result;

fail:
  if (path)
    free(path);

  return 1;
}
