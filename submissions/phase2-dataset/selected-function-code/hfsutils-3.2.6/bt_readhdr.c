{
  const byte *ptr;
  byte *map = 0;
  int i;
  unsigned long nnum;

  if (bt_getnode(&bt->hdrnd, bt, 0) == -1)
    goto fail;

  if (bt->hdrnd.nd.ndType != ndHdrNode ||
      bt->hdrnd.nd.ndNRecs != 3 ||
      bt->hdrnd.roff[0] != 0x00e ||
      bt->hdrnd.roff[1] != 0x078 ||
      bt->hdrnd.roff[2] != 0x0f8 ||
      bt->hdrnd.roff[3] != 0x1f8)
    ERROR(EIO, "malformed b*-tree header node");

  /* read header record */

  ptr = HFS_NODEREC(bt->hdrnd, 0);

  d_fetchuw(&ptr, &bt->hdr.bthDepth);
  d_fetchul(&ptr, &bt->hdr.bthRoot);
  d_fetchul(&ptr, &bt->hdr.bthNRecs);
  d_fetchul(&ptr, &bt->hdr.bthFNode);
  d_fetchul(&ptr, &bt->hdr.bthLNode);
  d_fetchuw(&ptr, &bt->hdr.bthNodeSize);
  d_fetchuw(&ptr, &bt->hdr.bthKeyLen);
  d_fetchul(&ptr, &bt->hdr.bthNNodes);
  d_fetchul(&ptr, &bt->hdr.bthFree);

  for (i = 0; i < 76; ++i)
    d_fetchsb(&ptr, &bt->hdr.bthResv[i]);

  if (bt->hdr.bthNodeSize != HFS_BLOCKSZ)
    ERROR(EINVAL, "unsupported b*-tree node size");

  /* read map record; construct btree bitmap */
  /* don't set bt->map until we're done, since getnode() checks it */

  map = ALLOC(byte, HFS_MAP1SZ);
  if (map == 0)
    ERROR(ENOMEM, 0);

  memcpy(map, HFS_NODEREC(bt->hdrnd, 2), HFS_MAP1SZ);
  bt->mapsz = HFS_MAP1SZ;

  /* read continuation map records, if any */

  nnum = bt->hdrnd.nd.ndFLink;

  while (nnum)
    {
      node n;
      byte *newmap;

      if (bt_getnode(&n, bt, nnum) == -1)
	goto fail;

      if (n.nd.ndType != ndMapNode ||
	  n.nd.ndNRecs != 1 ||
	  n.roff[0] != 0x00e ||
	  n.roff[1] != 0x1fa)
	ERROR(EIO, "malformed b*-tree map node");

      newmap = REALLOC(map, byte, bt->mapsz + HFS_MAPXSZ);
      if (newmap == 0)
	ERROR(ENOMEM, 0);

      map = newmap;

      memcpy(map + bt->mapsz, HFS_NODEREC(n, 0), HFS_MAPXSZ);
      bt->mapsz += HFS_MAPXSZ;

      nnum = n.nd.ndFLink;
    }

  bt->map = map;

  return 0;

fail:
  FREE(map);
  return -1;
}
