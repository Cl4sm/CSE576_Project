int hfs_truncate(hfsfile *file, unsigned long len)
{
  unsigned long *lglen;

  f_getptrs(file, 0, &lglen, 0);

  if (*lglen > len)
    {
      if (file->vol->flags & HFS_VOL_READONLY)
	ERROR(EROFS, 0);

      *lglen = len;

      file->cat.u.fil.filMdDat = d_mtime(time(0));
      file->flags |= HFS_FILE_UPDATE_CATREC;

      if (file->pos > len)
	file->pos = len;
    }

  return 0;

fail:
  return -1;
}
