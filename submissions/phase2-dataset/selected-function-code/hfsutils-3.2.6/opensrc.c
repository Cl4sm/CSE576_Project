static
hfsfile *opensrc(hfsvol *vol, const char *srcname,
		 const char **dsthint, const char *ext)
{
  hfsfile *file;
  hfsdirent ent;
  static char name[HFS_MAX_FLEN + 4 + 1];
  char *ptr;

  file = hfs_open(vol, srcname);
  if (file == 0)
    {
      ERROR(errno, hfs_error);
      return 0;
    }

  if (hfs_fstat(file, &ent) == -1)
    {
      ERROR(errno, hfs_error);

      hfs_close(file);
      return 0;
    }

  strcpy(name, ent.name);

  for (ptr = name; *ptr; ++ptr)
    {
      switch (*ptr)
	{
	case '/':
	  *ptr = '-';
	  break;

	case ' ':
	  *ptr = '_';
	  break;
	}
    }

  if (ext)
    strcat(name, ext);

  *dsthint = name;

  return file;
}
