int do_copyin(hfsvol *vol, int argc, char *argv[], const char *dest, int mode)
{
  hfsdirent ent;
  struct stat sbuf;
  cpifunc copyfile;
  int i, result = 0;

  if (argc > 1 && (hfs_stat(vol, dest, &ent) == -1 ||
		   ! (ent.flags & HFS_ISDIR)))
    {
      ERROR(ENOTDIR, 0);
      hfsutil_perrorp(dest);

      return 1;
    }

  switch (mode)
    {
    case 'm':
      copyfile = cpi_macb;
      break;

    case 'b':
      copyfile = cpi_binh;
      break;

    case 't':
      copyfile = cpi_text;
      break;

    case 'r':
      copyfile = cpi_raw;
      break;
    }

  for (i = 0; i < argc; ++i)
    {
      if (stat(argv[i], &sbuf) != -1 &&
	  S_ISDIR(sbuf.st_mode))
	{
	  ERROR(EISDIR, 0);
	  hfsutil_perrorp(argv[i]);

	  result = 1;
	}
      else
	{
	  if (mode == 'a')
	    copyfile = automode_unix(argv[i]);

	  if (copyfile(argv[i], vol, dest) == -1)
	    {
	      ERROR(errno, cpi_error);
	      hfsutil_perrorp(argv[i]);

	      result = 1;
	    }
	}
    }

  return result;
}
