val_same(struct f_val v1, struct f_val v2)
{
  int rc;

  rc = val_compare(v1, v2);
  if (rc != CMP_ERROR)
    return !rc;

  if (v1.type != v2.type)
    return 0;

  switch (v1.type) {
  case T_PATH_MASK:
    return pm_path_same(v1.val.path_mask, v2.val.path_mask);
  case T_PATH:
  case T_CLIST:
  case T_ECLIST:
    return adata_same(v1.val.ad, v2.val.ad);
  case T_SET:
    return same_tree(v1.val.t, v2.val.t);
  case T_PREFIX_SET:
    return trie_same(v1.val.ti, v2.val.ti);
  default:
    bug("Invalid type in val_same(): %x", v1.type);
  }
}
