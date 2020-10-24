int bt_writehdr(btree *bt)
{
  byte *ptr, *map;
  unsigned long mapsz, nnum;
  int i;

  ASSERT(bt->hdrnd.bt == bt &&
	 bt->hdrnd.nnum == 0 &&
	 bt->hdrnd.nd.ndType == ndHdrNode &&
	 bt->hdrnd.nd.ndNRecs == 3);

  ptr = HFS_NODEREC(bt->hdrnd, 0);

  d_storeuw(&ptr, bt->hdr.bthDepth);
  d_storeul(&ptr, bt->hdr.bthRoot);
  d_storeul(&ptr, bt->hdr.bthNRecs);
  d_storeul(&ptr, bt->hdr.bthFNode);
  d_storeul(&ptr, bt->hdr.bthLNode);
  d_storeuw(&ptr, bt->hdr.bthNodeSize);
  d_storeuw(&ptr, bt->hdr.bthKeyLen);
  d_storeul(&ptr, bt->hdr.bthNNodes);
  d_storeul(&ptr, bt->hdr.bthFree);

  for (i = 0; i < 76; ++i)
    d_storesb(&ptr, bt->hdr.bthResv[i]);

  memcpy(HFS_NODEREC(bt->hdrnd, 2), bt->map, HFS_MAP1SZ);

  if (bt_putnode(&bt->hdrnd) == -1)
    goto fail;

  map   = bt->map   + HFS_MAP1SZ;
  mapsz = bt->mapsz - HFS_MAP1SZ;

  nnum  = bt->hdrnd.nd.ndFLink;

  while (mapsz)
    {
      node n;

      if (nnum == 0)
	ERROR(EIO, "truncated b*-tree map");

      if (bt_getnode(&n, bt, nnum) == -1)
	goto fail;

      if (n.nd.ndType != ndMapNode ||
	  n.nd.ndNRecs != 1 ||
	  n.roff[0] != 0x00e ||
	  n.roff[1] != 0x1fa)
	ERROR(EIO, "malformed b*-tree map node");

      memcpy(HFS_NODEREC(n, 0), map, HFS_MAPXSZ);

      if (bt_putnode(&n) == -1)
	goto fail;

      map   += HFS_MAPXSZ;
      mapsz -= HFS_MAPXSZ;

      nnum = n.nd.ndFLink;
    }

  bt->flags &= ~HFS_BT_UPDATE_HDR;

  return 0;

fail:
  return -1;
}
