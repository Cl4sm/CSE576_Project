int b_writepb(hfsvol *vol, unsigned long bnum, const block *bp,
	      unsigned int blen)
{
  unsigned long nblocks;

# ifdef DEBUG
  fprintf(stderr, "BLOCK: WRITE vol 0x%lx block %lu",
	  (unsigned long) vol, bnum);
  if (blen > 1)
    fprintf(stderr, "+%u[..%lu]\n", blen - 1, bnum + blen - 1);
  else
    fprintf(stderr, "\n");
# endif

  nblocks = os_seek(&vol->priv, bnum);
  if (nblocks == (unsigned long) -1)
    goto fail;

  if (nblocks != bnum)
    ERROR(EIO, "block seek failed for write");

  nblocks = os_write(&vol->priv, bp, blen);
  if (nblocks == (unsigned long) -1)
    goto fail;

  if (nblocks != blen)
    ERROR(EIO, "incomplete block write");

  return 0;

fail:
  return -1;
}
