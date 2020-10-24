refs_cmp (const void *A, const void *B)
{
  struct prelink_entry *a = * (struct prelink_entry **) A;
  struct prelink_entry *b = * (struct prelink_entry **) B;
  int i;

  /* Dynamic linkers first.  */
  if (! a->ndepends && b->ndepends)
    return -1;
  if (a->ndepends && ! b->ndepends)
    return 1;
  /* Most widely used libraries first.  */
  if (a->refs > b->refs)
    return -1;
  if (a->refs < b->refs)
    return 1;
  /* Largest libraries first.  */
  if (a->layend - a->base > b->layend - b->base)
    return -1;
  if (a->layend - a->base < b->layend - b->base)
    return 1;
  if (a->refs)
    {
      i = strcmp (a->soname, b->soname);
      if (i)
	return i;
    }
  return strcmp (a->filename, b->filename);
}
