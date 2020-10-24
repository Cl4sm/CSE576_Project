{
  unsigned long low, high, mid;
  block b;

  high = os_seek(&vol->priv, -1);

  if (high != (unsigned long) -1 && high > 0)
    return high;

  /* manual size detection: first check there is at least 1 block in medium */

  if (b_readpb(vol, 0, &b, 1) == -1)
    ERROR(EIO, "size of medium indeterminable or empty");

  for (low = 0, high = 2880;
       high > 0 && b_readpb(vol, high - 1, &b, 1) != -1;
       high <<= 1)
    low = high - 1;

  if (high == 0)
    ERROR(EIO, "size of medium indeterminable or too large");

  /* common case: 1440K floppy */

  if (low == 2879 && b_readpb(vol, 2880, &b, 1) == -1)
    return 2880;

  /* binary search for other sizes */

  while (low < high - 1)
    {
      mid = (low + high) >> 1;

      if (b_readpb(vol, mid, &b, 1) == -1)
	high = mid;
      else
	low = mid;
    }

  return low + 1;

fail:
  return 0;
}
