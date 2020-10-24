{
  mountent *ent;

  if (argc != 1)
    {
      fprintf(stderr, "Usage: %s\n", argv0);
      return 1;
    }

  ent = hcwd_getvol(-1);
  if (ent == 0)
    {
      fprintf(stderr, "%s: No volume is current; use `hmount' or `hvol'\n",
	      argv0);
      return 1;
    }

  if (strcmp(ent->cwd, ":") == 0)
    printf("%s:\n", ent->vname);
  else
    printf("%s%s:\n", ent->vname, ent->cwd);

  return 0;
}
