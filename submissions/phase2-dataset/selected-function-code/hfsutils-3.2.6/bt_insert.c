int bt_insert(btree *bt, const byte *record, unsigned int reclen)
{
  node root;
  byte newrec[HFS_MAX_RECLEN];

  if (bt->hdr.bthRoot == 0)
    {
      /* create root node */

      n_init(&root, bt, ndLeafNode, 1);
      if (n_new(&root) == -1 ||
	  bt_putnode(&root) == -1)
	goto fail;

      bt->hdr.bthDepth = 1;
      bt->hdr.bthRoot  = root.nnum;
      bt->hdr.bthFNode = root.nnum;
      bt->hdr.bthLNode = root.nnum;

      bt->flags |= HFS_BT_UPDATE_HDR;
    }
  else if (bt_getnode(&root, bt, bt->hdr.bthRoot) == -1)
    goto fail;

  memcpy(newrec, record, reclen);

  if (insertx(&root, newrec, &reclen) == -1)
    goto fail;

  if (reclen)
    {
      byte oroot[HFS_MAX_RECLEN];
      unsigned int orootlen;

      /* root node was split; create a new root */

      n_index(&root, oroot, &orootlen);

      n_init(&root, bt, ndIndxNode, root.nd.ndNHeight + 1);
      if (n_new(&root) == -1)
	goto fail;

      ++bt->hdr.bthDepth;
      bt->hdr.bthRoot = root.nnum;

      bt->flags |= HFS_BT_UPDATE_HDR;

      /* insert index records for new root */

      n_search(&root, oroot);
      n_insertx(&root, oroot, orootlen);

      n_search(&root, newrec);
      n_insertx(&root, newrec, reclen);

      if (bt_putnode(&root) == -1)
	goto fail;
    }

  ++bt->hdr.bthNRecs;
  bt->flags |= HFS_BT_UPDATE_HDR;

  return 0;

fail:
  return -1;
}
