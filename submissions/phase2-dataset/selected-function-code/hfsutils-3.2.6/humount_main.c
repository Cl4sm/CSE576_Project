{
  int vnum;
  mountent *ent;

  if (argc > 2)
    {
      fprintf(stderr, "Usage: %s [volume-name-or-path]\n", argv0);
      return 1;
    }

  if (argc == 1)
    {
      if (hcwd_umounted(-1) == -1 &&
	  hcwd_getvol(-1) == 0)
	{
	  fprintf(stderr, "%s: No volume is current\n", argv0);
	  return 1;
	}

      return 0;
    }

  for (ent = hcwd_getvol(vnum = 0); ent; ent = hcwd_getvol(++vnum))
    {
      if (hfsutil_samepath(argv[1], ent->path) ||
	  strcasecmp(argv[1], ent->vname) == 0)
	{
	  hcwd_umounted(vnum);
	  return 0;
	}
    }

  fprintf(stderr, "%s: Unknown volume \"%s\"\n", argv0, argv[1]);

  return 1;
}
