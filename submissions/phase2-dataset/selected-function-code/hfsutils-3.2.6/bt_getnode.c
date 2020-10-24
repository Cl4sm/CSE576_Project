{
  block *bp = &np->data;
  const byte *ptr;
  int i;

  np->bt   = bt;
  np->nnum = nnum;

# if 0
  fprintf(stderr, "BTREE: GET vol \"%s\" btree \"%s\" node %lu\n",
	  bt->f.vol->mdb.drVN, bt->f.name, np->nnum);
# endif

  /* verify the node exists and is marked as in-use */

  if (nnum > 0 && nnum >= bt->hdr.bthNNodes)
    ERROR(EIO, "read nonexistent b*-tree node");
  else if (bt->map && ! BMTST(bt->map, nnum))
    ERROR(EIO, "read unallocated b*-tree node");

  if (f_getblock(&bt->f, nnum, bp) == -1)
    goto fail;

  ptr = *bp;

  d_fetchul(&ptr, &np->nd.ndFLink);
  d_fetchul(&ptr, &np->nd.ndBLink);
  d_fetchsb(&ptr, &np->nd.ndType);
  d_fetchsb(&ptr, &np->nd.ndNHeight);
  d_fetchuw(&ptr, &np->nd.ndNRecs);
  d_fetchsw(&ptr, &np->nd.ndResv2);

  if (np->nd.ndNRecs > HFS_MAX_NRECS)
    ERROR(EIO, "too many b*-tree node records");

  i = np->nd.ndNRecs + 1;

  ptr = *bp + HFS_BLOCKSZ - (2 * i);

  while (i--)
    d_fetchuw(&ptr, &np->roff[i]);

  return 0;

fail:
  return -1;
}
