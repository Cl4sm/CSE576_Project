struct adata *
int_set_del(struct linpool *pool, struct adata *list, u32 val)
{
  if (!int_set_contains(list, val))
    return list;

  struct adata *res;
  res = lp_alloc(pool, sizeof(struct adata) + list->length - 4);
  res->length = list->length - 4;

  u32 *l = int_set_get_data(list);
  u32 *k = int_set_get_data(res);
  int len = int_set_get_size(list);
  int i;

  for (i = 0; i < len; i++)
    if (l[i] != val)
      *k++ = l[i];

  return res;
}
