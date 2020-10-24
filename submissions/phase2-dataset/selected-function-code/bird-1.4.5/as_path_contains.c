as_path_contains(struct adata *path, u32 as, int min)
{
  u8 *p = path->data;
  u8 *q = p+path->length;
  int num = 0;
  int i, n;

  while (p<q)
    {
      n = p[1];
      p += 2;
      for(i=0; i<n; i++)
	{
	  if (get_as(p) == as)
	    if (++num == min)
	      return 1;
	  p += BS;
	}
    }
  return 0;
}
