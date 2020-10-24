static int
refs_rnd_cmp (const void *A, const void *B)
{
  struct prelink_entry *a = * (struct prelink_entry **) A;
  struct prelink_entry *b = * (struct prelink_entry **) B;
  int i, refs;

  /* Dynamic linkers first.  */
  if (! a->ndepends && b->ndepends)
    return -1;
  if (a->ndepends && ! b->ndepends)
    return 1;
  /* Most widely used libraries first with some randomization.  */
  refs = a->refs < b->refs ? a->refs : b->refs;
  if (refs < 8)
    i = 1;
  else if (refs < 32)
    i = 2;
  else if (refs < 128)
    i = 4;
  else
    i = 8;
  if (a->refs > b->refs && a->refs - b->refs >= i)
    return -1;
  if (a->refs < b->refs && b->refs - a->refs >= i)
    return 1;
  if (a->u.tmp < b->u.tmp)
    return -1;
  if (a->u.tmp > b->u.tmp)
    return 1;
  /* Largest libraries first.  */
  if (a->layend - a->base > b->layend - b->base)
    return -1;
  if (a->layend - a->base < b->layend - b->base)
    return 1;
  if (a->refs && b->refs)
    {
      i = strcmp (a->soname, b->soname);
      if (i)
	return i;
    }
  return strcmp (a->filename, b->filename);
}
