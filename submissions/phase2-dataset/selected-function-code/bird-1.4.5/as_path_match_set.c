int
as_path_match_set(struct adata *path, struct f_tree *set)
{
  u8 *p = path->data;
  u8 *q = p+path->length;
  int i, n;

  while (p<q)
    {
      n = p[1];
      p += 2;
      for (i=0; i<n; i++)
	{
	  struct f_val v = {T_INT, .val.i = get_as(p)};
	  if (find_tree(set, v))
	    return 1;
	  p += BS;
	}
    }

  return 0;
}
