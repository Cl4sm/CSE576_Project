int b_writelb(hfsvol *vol, unsigned long bnum, const block *bp)
{
  if (vol->vlen > 0 && bnum >= vol->vlen)
    ERROR(EIO, "write nonexistent logical block");

  if (vol->cache)
    {
      bucket *b;

      b = getbucket(vol->cache, bnum, 0);
      if (b == 0)
	goto fail;

      if (! INUSE(b) ||
	  memcmp(b->data, bp, HFS_BLOCKSZ) != 0)
	{
	  memcpy(b->data, bp, HFS_BLOCKSZ);
	  b->flags |= HFS_BUCKET_INUSE | HFS_BUCKET_DIRTY;
	}
    }
  else
    {
      if (b_writepb(vol, vol->vstart + bnum, bp, 1) == -1)
	goto fail;
    }

  return 0;

fail:
  return -1;
}
