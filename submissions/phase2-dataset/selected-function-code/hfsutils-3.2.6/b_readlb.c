{
  if (vol->vlen > 0 && bnum >= vol->vlen)
    ERROR(EIO, "read nonexistent logical block");

  if (vol->cache)
    {
      bucket *b;

      b = getbucket(vol->cache, bnum, 1);
      if (b == 0)
	goto fail;

      memcpy(bp, b->data, HFS_BLOCKSZ);
    }
  else
    {
      if (b_readpb(vol, vol->vstart + bnum, bp, 1) == -1)
	goto fail;
    }

  return 0;

fail:
  return -1;
}
