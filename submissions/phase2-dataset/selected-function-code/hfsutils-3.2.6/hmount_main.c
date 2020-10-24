{
  char *path = 0;
  hfsvol *vol;
  hfsvolent ent;
  int nparts, partno, result = 0;

  if (argc < 2 || argc > 3)
    {
      fprintf(stderr, "Usage: %s source-path [partition-no]\n", argv0);
      goto fail;
    }

  path = hfsutil_abspath(argv[1]);
  if (path == 0)
    {
      fprintf(stderr, "%s: not enough memory\n", argv0);
      goto fail;
    }

  suid_enable();
  nparts = hfs_nparts(path);
  suid_disable();

  if (nparts >= 0)
    printf("%s: contains %d HFS partition%s\n", path, nparts,
	   nparts == 1 ? "" : "s");

  if (argc == 3)
    partno = atoi(argv[2]);
  else
    {
      if (nparts > 1)
	{
	  fprintf(stderr, "%s: must specify partition number\n", argv0);
	  goto fail;
	}
      else if (nparts == -1)
	partno = 0;
      else
	partno = 1;
    }

  suid_enable();
  vol = hfs_mount(path, partno, HFS_MODE_ANY);
  suid_disable();

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
