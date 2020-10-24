ec_set_del(struct linpool *pool, struct adata *list, u64 val)
{
  if (!ec_set_contains(list, val))
    return list;

  struct adata *res;
  res = lp_alloc(pool, sizeof(struct adata) + list->length - 8);
  res->length = list->length - 8;

  u32 *l = int_set_get_data(list);
  u32 *k = int_set_get_data(res);
  int len = int_set_get_size(list);
  u32 eh = ec_hi(val);
  u32 el = ec_lo(val);
  int i;

  for (i=0; i < len; i += 2)
    if (! (l[i] == eh && l[i+1] == el))
      {
	*k++ = l[i];
	*k++ = l[i+1];
      }

  return res;
}
