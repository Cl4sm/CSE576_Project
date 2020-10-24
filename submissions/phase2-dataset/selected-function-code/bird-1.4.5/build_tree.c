build_tree(struct f_tree *from)
{
  struct f_tree *tmp, *root;
  struct f_tree **buf;
  int len, i;

  if (from == NULL)
    return NULL;

  len = 0;
  for (tmp = from; tmp != NULL; tmp = tmp->left)
    len++;

  if (len <= 1024)
    buf = alloca(len * sizeof(struct f_tree *));
  else
    buf = malloc(len * sizeof(struct f_tree *));

  /* Convert a degenerated tree into an sorted array */
  i = 0;
  for (tmp = from; tmp != NULL; tmp = tmp->left)
    buf[i++] = tmp;

  qsort(buf, len, sizeof(struct f_tree *), tree_compare);

  root = build_tree_rec(buf, 0, len);

  if (len > 1024)
    free(buf);

  return root;
}
