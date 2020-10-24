int do_copyout(hfsvol *vol, int argc, char *argv[], const char *dest, int mode)
{
  struct stat sbuf;
  hfsdirent ent;
  cpofunc copyfile;
  int i, result = 0;

  if (argc > 1 && (stat(dest, &sbuf) == -1 ||
		   ! S_ISDIR(sbuf.st_mode)))
    {
      ERROR(ENOTDIR, 0);
      hfsutil_perrorp(dest);

      return 1;
    }

  switch (mode)
    {
    case 'm':
      copyfile = cpo_macb;
      break;

    case 'b':
      copyfile = cpo_binh;
      break;

    case 't':
      copyfile = cpo_text;
      break;

    case 'r':
      copyfile = cpo_raw;
      break;
    }

  for (i = 0; i < argc; ++i)
    {
      if (hfs_stat(vol, argv[i], &ent) != -1 &&
	  (ent.flags & HFS_ISDIR))
	{
	  ERROR(EISDIR, 0);
	  hfsutil_perrorp(argv[i]);

	  result = 1;
	}
      else
	{
	  if (mode == 'a')
	    copyfile = automode_hfs(vol, argv[i]);

	  if (copyfile(vol, argv[i], dest) == -1)
	    {
	      ERROR(errno, cpo_error);
	      hfsutil_perrorp(argv[i]);

	      result = 1;
	    }
	}
    }

  return result;
}
