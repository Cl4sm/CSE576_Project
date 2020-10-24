clist_filter(struct linpool *pool, struct adata *list, struct f_val set, int pos)
{
  if (!list)
    return NULL;

  int tree = (set.type == T_SET);	/* 1 -> set is T_SET, 0 -> set is T_CLIST */
  struct f_val v;
  if (tree)
    clist_set_type(set.val.t, &v);
  else
    v.type = T_PAIR;

  int len = int_set_get_size(list);
  u32 *l = int_set_get_data(list);
  u32 tmp[len];
  u32 *k = tmp;
  u32 *end = l + len;

  while (l < end) {
    v.val.i = *l++;
    /* pos && member(val, set) || !pos && !member(val, set),  member() depends on tree */
    if ((tree ? !!find_tree(set.val.t, v) : int_set_contains(set.val.ad, v.val.i)) == pos)
      *k++ = v.val.i;
  }

  int nl = (k - tmp) * 4;
  if (nl == list->length)
    return list;

  struct adata *res = adata_empty(pool, nl);
  memcpy(res->data, tmp, nl);
  return res;
}
