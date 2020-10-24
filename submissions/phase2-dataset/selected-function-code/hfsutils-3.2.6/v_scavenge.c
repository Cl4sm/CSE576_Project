int v_scavenge(hfsvol *vol)
{
  block *vbm = vol->vbm;
  node n;
  unsigned int pt, blks;
  unsigned long lastcnid = 15;

# ifdef DEBUG
  fprintf(stderr, "VOL: \"%s\" not cleanly unmounted\n",
	  vol->mdb.drVN);
# endif

  if (vol->flags & HFS_VOL_READONLY)
    goto done;

# ifdef DEBUG
  fprintf(stderr, "VOL: scavenging...\n");
# endif

  /* reset MDB by marking it dirty again */

  vol->mdb.drAtrb |= HFS_ATRB_UMOUNTED;
  if (v_dirty(vol) == -1)
    goto fail;

  /* begin by marking extents in MDB */

  markexts(vbm, &vol->mdb.drXTExtRec);
  markexts(vbm, &vol->mdb.drCTExtRec);

  vol->flags |= HFS_VOL_UPDATE_VBM;

  /* scavenge the extents overflow file */

  if (vol->ext.hdr.bthFNode > 0)
    {
      if (bt_getnode(&n, &vol->ext, vol->ext.hdr.bthFNode) == -1)
	goto fail;

      n.rnum = 0;

      while (1)
	{
	  ExtDataRec data;
	  const byte *ptr;

	  while (n.rnum >= n.nd.ndNRecs && n.nd.ndFLink > 0)
	    {
	      if (bt_getnode(&n, &vol->ext, n.nd.ndFLink) == -1)
		goto fail;

	      n.rnum = 0;
	    }

	  if (n.rnum >= n.nd.ndNRecs && n.nd.ndFLink == 0)
	    break;

	  ptr = HFS_NODEREC(n, n.rnum);
	  r_unpackextdata(HFS_RECDATA(ptr), &data);

	  markexts(vbm, &data);

	  ++n.rnum;
	}
    }

  /* scavenge the catalog file */

  if (vol->cat.hdr.bthFNode > 0)
    {
      if (bt_getnode(&n, &vol->cat, vol->cat.hdr.bthFNode) == -1)
	goto fail;

      n.rnum = 0;

      while (1)
	{
	  CatDataRec data;
	  const byte *ptr;

	  while (n.rnum >= n.nd.ndNRecs && n.nd.ndFLink > 0)
	    {
	      if (bt_getnode(&n, &vol->cat, n.nd.ndFLink) == -1)
		goto fail;

	      n.rnum = 0;
	    }

	  if (n.rnum >= n.nd.ndNRecs && n.nd.ndFLink == 0)
	    break;

	  ptr = HFS_NODEREC(n, n.rnum);
	  r_unpackcatdata(HFS_RECDATA(ptr), &data);

	  switch (data.cdrType)
	    {
	    case cdrFilRec:
	      markexts(vbm, &data.u.fil.filExtRec);
	      markexts(vbm, &data.u.fil.filRExtRec);

	      if (data.u.fil.filFlNum > lastcnid)
		lastcnid = data.u.fil.filFlNum;
	      break;

	    case cdrDirRec:
	      if (data.u.dir.dirDirID > lastcnid)
		lastcnid = data.u.dir.dirDirID;
	      break;
	    }

	  ++n.rnum;
	}
    }

  /* count free blocks */

  for (blks = 0, pt = vol->mdb.drNmAlBlks; pt--; )
    {
      if (! BMTST(vbm, pt))
	++blks;
    }

  if (vol->mdb.drFreeBks != blks)
    {
# ifdef DEBUG
      fprintf(stderr, "VOL: updating free blocks from %u to %u\n",
	      vol->mdb.drFreeBks, blks);
# endif

      vol->mdb.drFreeBks = blks;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  /* ensure next CNID is sane */

  if ((unsigned long) vol->mdb.drNxtCNID <= lastcnid)
    {
# ifdef DEBUG
      fprintf(stderr, "VOL: updating next CNID from %lu to %lu\n",
	      vol->mdb.drNxtCNID, lastcnid + 1);
# endif

      vol->mdb.drNxtCNID = lastcnid + 1;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

# ifdef DEBUG
  fprintf(stderr, "VOL: scavenging complete\n");
# endif

done:
  return 0;

fail:
  return -1;
}
