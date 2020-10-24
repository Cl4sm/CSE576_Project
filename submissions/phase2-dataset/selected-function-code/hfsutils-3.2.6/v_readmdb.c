{
  if (l_getmdb(vol, &vol->mdb, 0) == -1)
    goto fail;

  if (vol->mdb.drSigWord != HFS_SIGWORD)
    {
      if (vol->mdb.drSigWord == HFS_SIGWORD_MFS)
	ERROR(EINVAL, "MFS volume format not supported");
      else
	ERROR(EINVAL, "not a Macintosh HFS volume");
    }

  if (vol->mdb.drAlBlkSiz % HFS_BLOCKSZ != 0)
    ERROR(EINVAL, "bad volume allocation block size");

  vol->lpa = vol->mdb.drAlBlkSiz >> HFS_BLOCKSZ_BITS;

  /* extents pseudo-file structs */

  vol->ext.f.cat.u.fil.filStBlk = vol->mdb.drXTExtRec[0].xdrStABN;
  vol->ext.f.cat.u.fil.filLgLen = vol->mdb.drXTFlSize;
  vol->ext.f.cat.u.fil.filPyLen = vol->mdb.drXTFlSize;

  vol->ext.f.cat.u.fil.filCrDat = vol->mdb.drCrDate;
  vol->ext.f.cat.u.fil.filMdDat = vol->mdb.drLsMod;

  memcpy(&vol->ext.f.cat.u.fil.filExtRec,
	 &vol->mdb.drXTExtRec, sizeof(ExtDataRec));

  f_selectfork(&vol->ext.f, fkData);

  /* catalog pseudo-file structs */

  vol->cat.f.cat.u.fil.filStBlk = vol->mdb.drCTExtRec[0].xdrStABN;
  vol->cat.f.cat.u.fil.filLgLen = vol->mdb.drCTFlSize;
  vol->cat.f.cat.u.fil.filPyLen = vol->mdb.drCTFlSize;

  vol->cat.f.cat.u.fil.filCrDat = vol->mdb.drCrDate;
  vol->cat.f.cat.u.fil.filMdDat = vol->mdb.drLsMod;

  memcpy(&vol->cat.f.cat.u.fil.filExtRec,
	 &vol->mdb.drCTExtRec, sizeof(ExtDataRec));

  f_selectfork(&vol->cat.f, fkData);

  return 0;

fail:
  return -1;
}
