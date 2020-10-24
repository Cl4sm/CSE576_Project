static
int queuepath(hfsvol *vol, char *path, darray *dirs, darray *files, int flags)
{
  queueent ent;
  darray *array;

  if (hfs_stat(vol, path, &ent.dirent) == -1)
    {
      hfsutil_perrorp(path);
      return (errno == ENOENT) ? 0 : -1;
    }

  ent.path = path;
  ent.free = 0;

  array = ((ent.dirent.flags & HFS_ISDIR) &&
	   ! (flags & HLS_IMMEDIATE_DIRS)) ? dirs : files;

  if (darr_append(array, &ent) == 0)
    {
      fprintf(stderr, "%s: not enough memory\n", argv0);
      return -1;
    }

  return 0;
}
