{
  node root;
  byte record[HFS_MAX_RECLEN];
  int flag = 0;

  if (bt->hdr.bthRoot == 0)
    ERROR(EIO, "empty b*-tree");

  if (bt_getnode(&root, bt, bt->hdr.bthRoot) == -1 ||
      deletex(&root, key, record, &flag) == -1)
    goto fail;

  if (bt->hdr.bthDepth > 1 && root.nd.ndNRecs == 1)
    {
      const byte *rec;

      /* root only has one record; eliminate it and decrease the tree depth */

      rec = HFS_NODEREC(root, 0);

      --bt->hdr.bthDepth;
      bt->hdr.bthRoot = d_getul(HFS_RECDATA(rec));

      if (n_free(&root) == -1)
	goto fail;
    }
  else if (bt->hdr.bthDepth == 1 && root.nd.ndNRecs == 0)
    {
      /* root node was deleted */

      bt->hdr.bthDepth = 0;
      bt->hdr.bthRoot  = 0;
    }

  --bt->hdr.bthNRecs;
  bt->flags |= HFS_BT_UPDATE_HDR;

  return 0;

fail:
  return -1;
}
