unsigned long hfs_seek(hfsfile *file, long offset, int from)
{
  unsigned long *lglen, newpos;

  f_getptrs(file, 0, &lglen, 0);

  switch (from)
    {
    case HFS_SEEK_SET:
      newpos = (offset < 0) ? 0 : offset;
      break;

    case HFS_SEEK_CUR:
      if (offset < 0 && (unsigned long) -offset > file->pos)
	newpos = 0;
      else
	newpos = file->pos + offset;
      break;

    case HFS_SEEK_END:
      if (offset < 0 && (unsigned long) -offset > *lglen)
	newpos = 0;
      else
	newpos = *lglen + offset;
      break;

    default:
      ERROR(EINVAL, 0);
    }

  if (newpos > *lglen)
    newpos = *lglen;

  file->pos = newpos;

  return newpos;

fail:
  return -1;
}
