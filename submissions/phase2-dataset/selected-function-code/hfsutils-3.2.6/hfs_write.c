unsigned long hfs_write(hfsfile *file, const void *buf, unsigned long len)
{
  unsigned long *lglen, *pylen, count;
  const byte *ptr = buf;

  if (file->vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  f_getptrs(file, 0, &lglen, &pylen);

  count = len;

  /* set flag to update (at least) the modification time */

  if (count)
    {
      file->cat.u.fil.filMdDat = d_mtime(time(0));
      file->flags |= HFS_FILE_UPDATE_CATREC;
    }

  while (count)
    {
      unsigned long bnum, offs, chunk;

      bnum  = file->pos >> HFS_BLOCKSZ_BITS;
      offs  = file->pos & (HFS_BLOCKSZ - 1);

      chunk = HFS_BLOCKSZ - offs;
      if (chunk > count)
	chunk = count;

      if (file->pos + chunk > *pylen)
	{
	  if (bt_space(&file->vol->ext, 1) == -1 ||
	      f_alloc(file) == -1)
	    goto fail;
	}

      if (offs == 0 && chunk == HFS_BLOCKSZ)
	{
	  if (f_putblock(file, bnum, (block *) ptr) == -1)
	    goto fail;
	}
      else
	{
	  block b;

	  if (f_getblock(file, bnum, &b) == -1)
	    goto fail;

	  memcpy(b + offs, ptr, chunk);

	  if (f_putblock(file, bnum, &b) == -1)
	    goto fail;
	}

      ptr += chunk;

      file->pos += chunk;
      count     -= chunk;

      if (file->pos > *lglen)
	*lglen = file->pos;
    }

  return len;

fail:
  return -1;
}
