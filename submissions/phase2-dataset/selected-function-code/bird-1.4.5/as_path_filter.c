as_path_filter(struct linpool *pool, struct adata *path, struct f_tree *set, u32 key, int pos)
{
  if (!path)
    return NULL;

  int len = path->length;
  u8 *p = path->data;
  u8 *q = path->data + len;
  u8 *d, *d2;
  int i, bt, sn, dn;
  u8 buf[len];

  d = buf;
  while (p<q)
    {
      /* Read block header (type and length) */
      bt = p[0];
      sn = p[1];
      dn = 0;
      p += 2;
      d2 = d + 2;

      for (i = 0; i < sn; i++)
	{
	  u32 as = get_as(p);
	  int match;

	  if (set)
	    match = !!find_tree(set, (struct f_val){T_INT, .val.i = as});
	  else
	    match = (as == key);

	  if (match == pos)
	    {
	      put_as(d2, as);
	      d2 += BS;
	      dn++;
	    }

	  p += BS;
	}

      if (dn > 0)
	{
	  /* Nonempty block, set block header and advance */
	  d[0] = bt;
	  d[1] = dn;
	  d = d2;
	}
  }

  int nl = d - buf;
  if (nl == path->length)
    return path;

  struct adata *res = lp_alloc(pool, sizeof(struct adata) + nl);
  res->length = nl;
  memcpy(res->data, buf, nl);

  return res;
}
