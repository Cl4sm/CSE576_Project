{
  int fd = (int) *priv;
  off_t result;

  /* offset == -1 special; seek to last block of device */

  if (offset == (unsigned long) -1)
    result = lseek(fd, 0, SEEK_END);
  else
    result = lseek(fd, offset << HFS_BLOCKSZ_BITS, SEEK_SET);

  if (result == -1)
    ERROR(errno, "error seeking medium");

  return (unsigned long) result >> HFS_BLOCKSZ_BITS;

fail:
  return -1;
}
