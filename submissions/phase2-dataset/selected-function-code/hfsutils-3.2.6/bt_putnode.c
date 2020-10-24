int bt_putnode(node *np)
{
  btree *bt = np->bt;
  block *bp = &np->data;
  byte *ptr;
  int i;

# if 0
  fprintf(stderr, "BTREE: PUT vol \"%s\" btree \"%s\" node %lu\n",
	  bt->f.vol->mdb.drVN, bt->f.name, np->nnum);
# endif

  /* verify the node exists and is marked as in-use */

  if (np->nnum > 0 && np->nnum >= bt->hdr.bthNNodes)
    ERROR(EIO, "write nonexistent b*-tree node");
  else if (bt->map && ! BMTST(bt->map, np->nnum))
    ERROR(EIO, "write unallocated b*-tree node");

  ptr = *bp;

  d_storeul(&ptr, np->nd.ndFLink);
  d_storeul(&ptr, np->nd.ndBLink);
  d_storesb(&ptr, np->nd.ndType);
  d_storesb(&ptr, np->nd.ndNHeight);
  d_storeuw(&ptr, np->nd.ndNRecs);
  d_storesw(&ptr, np->nd.ndResv2);

  if (np->nd.ndNRecs > HFS_MAX_NRECS)
    ERROR(EIO, "too many b*-tree node records");

  i = np->nd.ndNRecs + 1;

  ptr = *bp + HFS_BLOCKSZ - (2 * i);

  while (i--)
    d_storeuw(&ptr, np->roff[i]);

  return f_putblock(&bt->f, np->nnum, bp);

fail:
  return -1;
}
