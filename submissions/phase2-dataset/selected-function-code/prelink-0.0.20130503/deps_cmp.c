deps_cmp (const void *A, const void *B)
{
  struct prelink_entry *a = * (struct prelink_entry **) A;
  struct prelink_entry *b = * (struct prelink_entry **) B;

  if (a == NULL)
    return (b != NULL);
  if (a != NULL && b == NULL)
    return -1;

  if (a->type == ET_NONE && b->type != ET_NONE)
    return 1;
  if (a->type != ET_NONE && b->type == ET_NONE)
    return -1;

  /* Libraries with fewest dependencies first.  */
  if (a->ndepends < b->ndepends)
    return -1;
  if (a->ndepends > b->ndepends)
    return 1;
  return 0;
}
