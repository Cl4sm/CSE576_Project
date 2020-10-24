unsigned long hfs_read(hfsfile *file, void *buf, unsigned long len)
{
  unsigned long *lglen, count;
  byte *ptr = buf;

  f_getptrs(file, 0, &lglen, 0);

  if (file->pos + len > *lglen)
    len = *lglen - file->pos;

  count = len;
  while (count)
    {
      unsigned long bnum, offs, chunk;

      bnum  = file->pos >> HFS_BLOCKSZ_BITS;
      offs  = file->pos & (HFS_BLOCKSZ - 1);

      chunk = HFS_BLOCKSZ - offs;
      if (chunk > count)
	chunk = count;

      if (offs == 0 && chunk == HFS_BLOCKSZ)
	{
	  if (f_getblock(file, bnum, (block *) ptr) == -1)
	    goto fail;
	}
      else
	{
	  block b;

	  if (f_getblock(file, bnum, &b) == -1)
	    goto fail;

	  memcpy(ptr, b + offs, chunk);
	}

      ptr += chunk;

      file->pos += chunk;
      count     -= chunk;
    }

  return len;

fail:
  return -1;
}
