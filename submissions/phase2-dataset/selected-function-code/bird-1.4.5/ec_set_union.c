struct adata *
ec_set_union(struct linpool *pool, struct adata *l1, struct adata *l2)
{
  if (!l1)
    return l2;
  if (!l2)
    return l1;

  struct adata *res;
  int len = int_set_get_size(l2);
  u32 *l = int_set_get_data(l2);
  u32 tmp[len];
  u32 *k = tmp;
  int i;

  for (i = 0; i < len; i += 2)
    if (!ec_set_contains(l1, ec_get(l, i)))
      {
	*k++ = l[i];
	*k++ = l[i+1];
      }

  if (k == tmp)
    return l1;

  len = (k - tmp) * 4;
  res = lp_alloc(pool, sizeof(struct adata) + l1->length + len);
  res->length = l1->length + len;
  memcpy(res->data, l1->data, l1->length);
  memcpy(res->data + l1->length, tmp, len);
  return res;
}
