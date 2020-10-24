{
  unsigned int nnodes;
  long space;

  nnodes = nrecs * (bt->hdr.bthDepth + 1);

  if (nnodes <= bt->hdr.bthFree)
    goto done;

  /* make sure the extents tree has room too */

  if (bt != &bt->f.vol->ext)
    {
      if (bt_space(&bt->f.vol->ext, 1) == -1)
	goto fail;
    }

  space = f_alloc(&bt->f);
  if (space == -1)
    goto fail;

  nnodes = space * (bt->f.vol->mdb.drAlBlkSiz / bt->hdr.bthNodeSize);

  bt->hdr.bthNNodes += nnodes;
  bt->hdr.bthFree   += nnodes;

  bt->flags |= HFS_BT_UPDATE_HDR;

  bt->f.vol->flags |= HFS_VOL_UPDATE_ALTMDB;

  while (bt->hdr.bthNNodes > bt->mapsz * 8)
    {
      byte *newmap;
      node mapnd;

      /* extend tree map */

      newmap = REALLOC(bt->map, byte, bt->mapsz + HFS_MAPXSZ);
      if (newmap == 0)
	ERROR(ENOMEM, 0);

      memset(newmap + bt->mapsz, 0, HFS_MAPXSZ);

      bt->map    = newmap;
      bt->mapsz += HFS_MAPXSZ;

      n_init(&mapnd, bt, ndMapNode, 0);
      if (n_new(&mapnd) == -1)
	goto fail;

      mapnd.nd.ndNRecs = 1;
      mapnd.roff[1]    = 0x1fa;

      /* link the new map node */

      if (bt->hdrnd.nd.ndFLink == 0)
	{
	  bt->hdrnd.nd.ndFLink = mapnd.nnum;
	  mapnd.nd.ndBLink     = 0;
	}
      else
	{
	  node n;
	  unsigned long nnum;

	  nnum = bt->hdrnd.nd.ndFLink;

	  while (1)
	    {
	      if (bt_getnode(&n, bt, nnum) == -1)
		goto fail;

	      if (n.nd.ndFLink == 0)
		break;

	      nnum = n.nd.ndFLink;
	    }

	  n.nd.ndFLink     = mapnd.nnum;
	  mapnd.nd.ndBLink = n.nnum;

	  if (bt_putnode(&n) == -1)
	    goto fail;
	}

      if (bt_putnode(&mapnd) == -1)
	goto fail;
    }

done:
  return 0;

fail:
  return -1;
}
