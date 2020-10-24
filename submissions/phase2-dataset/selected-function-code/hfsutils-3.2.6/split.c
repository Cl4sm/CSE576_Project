int split(node *left, byte *record, unsigned int *reclen)
{
  btree *bt = left->bt;
  node n, *right = &n, *side = 0;
  int mark, i;

  /* create a second node by cloning the first */

  *right = *left;

  if (n_new(right) == -1)
    goto fail;

  left->nd.ndFLink  = right->nnum;
  right->nd.ndBLink = left->nnum;

  /* divide all records evenly between the two nodes */

  mark = (NODEUSED(*left) + 2 * left->nd.ndNRecs + *reclen + 2) >> 1;

  if (left->rnum == -1)
    {
      side  = left;
      mark -= *reclen + 2;
    }

  for (i = 0; i < left->nd.ndNRecs; ++i)
    {
      node *np;
      byte *rec;

      np  = (mark > 0) ? right : left;
      rec = HFS_NODEREC(*np, i);

      mark -= HFS_RECLEN(*np, i) + 2;

      HFS_SETKEYLEN(rec, 0);

      if (left->rnum == i)
	{
	  side  = (mark > 0) ? left : right;
	  mark -= *reclen + 2;
	}
    }

  compact(left);
  compact(right);

  /* insert the new record and store the modified nodes */

  ASSERT(side);

  n_search(side, record);
  n_insertx(side, record, *reclen);

  if (bt_putnode(left) == -1 ||
      bt_putnode(right) == -1)
    goto fail;

  /* create an index record in the parent for the new node */

  n_index(right, record, reclen);

  /* update link pointers */

  if (bt->hdr.bthLNode == left->nnum)
    {
      bt->hdr.bthLNode = right->nnum;
      bt->flags |= HFS_BT_UPDATE_HDR;
    }

  if (right->nd.ndFLink > 0)
    {
      node sib;

      if (bt_getnode(&sib, right->bt, right->nd.ndFLink) == -1)
	goto fail;

      sib.nd.ndBLink = right->nnum;

      if (bt_putnode(&sib) == -1)
	goto fail;
    }

  return 0;

fail:
  return -1;
}
