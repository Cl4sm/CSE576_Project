static
int join(node *left, node *right, byte *record, int *flag)
{
  int i, offset;

  /* copy records and offsets */

  memcpy(HFS_NODEREC(*left, left->nd.ndNRecs),
	 HFS_NODEREC(*right, 0), NODEUSED(*right));

  offset = left->roff[left->nd.ndNRecs] - right->roff[0];

  for (i = 1; i <= right->nd.ndNRecs; ++i)
    left->roff[++left->nd.ndNRecs] = offset + right->roff[i];

  if (bt_putnode(left) == -1)
    goto fail;

  /* eliminate node and update link pointers */

  if (n_free(right) == -1)
    goto fail;

  HFS_SETKEYLEN(record, 0);
  *flag = 1;

  return 0;

fail:
  return -1;
}
