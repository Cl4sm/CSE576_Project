static int
addr_cmp (const void *A, const void *B)
{
  struct prelink_entry *a = * (struct prelink_entry **) A;
  struct prelink_entry *b = * (struct prelink_entry **) B;

  if (! a->done)
    return b->done ? 1 : 0;
  else if (! b->done)
    return -1;
  if (a->base < b->base)
    return -1;
  else if (a->base > b->base)
    return 1;
  if (a->layend < b->layend)
    return -1;
  else if (a->layend > b->layend)
    return 1;
  return 0;
}
