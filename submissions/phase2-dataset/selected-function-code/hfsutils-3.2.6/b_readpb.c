{
  unsigned long nblocks;

# ifdef DEBUG
  fprintf(stderr, "BLOCK: READ vol 0x%lx block %lu",
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
    ERROR(EIO, "block seek failed for read");

  nblocks = os_read(&vol->priv, bp, blen);
  if (nblocks == (unsigned long) -1)
    goto fail;

  if (nblocks != blen)
    ERROR(EIO, "incomplete block read");

  return 0;

fail:
  return -1;
}
