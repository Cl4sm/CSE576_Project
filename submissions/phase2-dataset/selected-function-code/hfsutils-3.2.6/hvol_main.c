{
  int vnum;
  mountent *ment;

  if (argc > 2)
    {
      fprintf(stderr, "Usage: %s [volume-name-or-path]\n", argv0);
      return 1;
    }

  if (argc == 1)
    {
      int output = 0, header = 0;

      ment = hcwd_getvol(-1);
      if (ment)
	{
	  showvol(ment);
	  output = 1;
	}

      for (vnum = 0; ; ++vnum)
	{
	  mountent *ent;

	  ent = hcwd_getvol(vnum);
	  if (ent == 0)
	    break;

	  if (ent == ment)
	    continue;

	  if (header == 0)
	    {
	      printf("%s volumes:\n", ment ? "\nOther known" : "Known");
	      header = 1;
	    }

	  if (ent->partno <= 0)
	    printf("  %-35s     \"%s\"\n", ent->path, ent->vname);
	  else
	    printf("  %-35s %2d  \"%s\"\n", ent->path, ent->partno,
		   ent->vname);

	  output = 1;
	}

      if (output == 0)
	printf("No known volumes; use `hmount' to introduce new volumes\n");

      return 0;
    }

  for (ment = hcwd_getvol(vnum = 0); ment; ment = hcwd_getvol(++vnum))
    {
      if (hfsutil_samepath(argv[1], ment->path) ||
	  strcasecmp(argv[1], ment->vname) == 0)
	{
	  hcwd_setvol(vnum);
	  return showvol(ment);
	}
    }

  fprintf(stderr, "%s: Unknown volume \"%s\"\n", argv0, argv[1]);

  return 1;
}
