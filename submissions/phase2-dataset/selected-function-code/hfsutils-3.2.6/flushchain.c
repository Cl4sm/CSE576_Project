static
int flushchain(hfsvol *vol, bucket **bptr, unsigned int *count)
{
  bucket *blist[HFS_BLOCKBUFSZ], **start = bptr;
  unsigned long bnum;
  unsigned int len, i;

  for (len = 0; len < HFS_BLOCKBUFSZ &&
	 (unsigned int) (bptr - start) < *count; ++bptr)
    {
      if (! INUSE(*bptr) || ! DIRTY(*bptr))
	continue;

      if (len > 0 && (*bptr)->bnum != bnum)
	break;

      blist[len++] = *bptr;
      bnum = (*bptr)->bnum + 1;
    }

  *count = bptr - start;

  if (len == 0)
    goto done;
  else if (len == 1)
    {
      if (b_writepb(vol, vol->vstart + blist[0]->bnum,
		    blist[0]->data, 1) == -1)
	goto fail;
    }
  else
    {
      block buffer[HFS_BLOCKBUFSZ];

      for (i = 0; i < len; ++i)
	memcpy(buffer[i], blist[i]->data, HFS_BLOCKSZ);

      if (b_writepb(vol, vol->vstart + blist[0]->bnum, buffer, len) == -1)
	goto fail;
    }

  for (i = 0; i < len; ++i)
    blist[i]->flags &= ~HFS_BUCKET_DIRTY;

done:
  return 0;

fail:
  return -1;
}
