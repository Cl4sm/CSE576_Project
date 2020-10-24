static
int do_rename(hfsvol *vol, int argc, char *argv[], const char *dest)
{
  hfsdirent ent;
  int i, result = 0;

  if (argc > 1 && (hfs_stat(vol, dest, &ent) == -1 ||
		   ! (ent.flags & HFS_ISDIR)))
    {
      ERROR(ENOTDIR, 0);
      hfsutil_perrorp(dest);

      return 1;
    }

  for (i = 0; i < argc; ++i)
    {
      if (hfs_rename(vol, argv[i], dest) == -1)
	{
	  hfsutil_perrorp(argv[i]);
	  result = 1;
	}
    }

  return result;
}
