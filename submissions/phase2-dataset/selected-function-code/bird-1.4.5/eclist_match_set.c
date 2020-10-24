eclist_match_set(struct adata *list, struct f_tree *set)
{
  if (!list)
    return 0;

  if (!eclist_set_type(set))
    return CMP_ERROR;

  struct f_val v;
  u32 *l = int_set_get_data(list);
  int len = int_set_get_size(list);
  int i;

  v.type = T_EC;
  for (i = 0; i < len; i += 2) {
    v.val.ec = ec_get(l, i);
    if (find_tree(set, v))
      return 1;
  }

  return 0;
}
