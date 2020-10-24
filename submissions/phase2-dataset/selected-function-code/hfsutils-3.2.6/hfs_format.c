	       unsigned int nbadblocks, const unsigned long badblocks[])
{
  hfsvol vol;
  btree *ext = &vol.ext;
  btree *cat = &vol.cat;
  unsigned int i, *badalloc = 0;

  v_init(&vol, mode);

  if (! validvname(vname))
    goto fail;

  if (v_open(&vol, path, HFS_MODE_RDWR) == -1 ||
      v_geometry(&vol, pnum) == -1)
    goto fail;

  /* initialize volume geometry */

  vol.lpa = 1 + ((vol.vlen - 6) >> 16);

  if (vol.flags & HFS_OPT_2048)
    vol.lpa = (vol.lpa + 3) & ~3;

  vol.vbmsz = (vol.vlen / vol.lpa + 0x0fff) >> 12;

  vol.mdb.drSigWord  = HFS_SIGWORD;
  vol.mdb.drCrDate   = d_mtime(time(0));
  vol.mdb.drLsMod    = vol.mdb.drCrDate;
  vol.mdb.drAtrb     = 0;
  vol.mdb.drNmFls    = 0;
  vol.mdb.drVBMSt    = 3;
  vol.mdb.drAllocPtr = 0;

  vol.mdb.drAlBlkSiz = vol.lpa << HFS_BLOCKSZ_BITS;
  vol.mdb.drClpSiz   = vol.mdb.drAlBlkSiz << 2;
  vol.mdb.drAlBlSt   = vol.mdb.drVBMSt + vol.vbmsz;

  if (vol.flags & HFS_OPT_2048)
    vol.mdb.drAlBlSt = ((vol.vstart & 3) + vol.mdb.drAlBlSt + 3) & ~3;

  vol.mdb.drNmAlBlks = (vol.vlen - 2 - vol.mdb.drAlBlSt) / vol.lpa;

  vol.mdb.drNxtCNID  = HFS_CNID_ROOTDIR;  /* modified later */
  vol.mdb.drFreeBks  = vol.mdb.drNmAlBlks;

  strcpy(vol.mdb.drVN, vname);

  vol.mdb.drVolBkUp  = 0;
  vol.mdb.drVSeqNum  = 0;
  vol.mdb.drWrCnt    = 0;

  vol.mdb.drXTClpSiz = vol.mdb.drNmAlBlks / 128 * vol.mdb.drAlBlkSiz;
  vol.mdb.drCTClpSiz = vol.mdb.drXTClpSiz;

  vol.mdb.drNmRtDirs = 0;
  vol.mdb.drFilCnt   = 0;
  vol.mdb.drDirCnt   = -1;  /* incremented when root directory is created */

  for (i = 0; i < 8; ++i)
    vol.mdb.drFndrInfo[i] = 0;

  vol.mdb.drEmbedSigWord            = 0x0000;
  vol.mdb.drEmbedExtent.xdrStABN    = 0;
  vol.mdb.drEmbedExtent.xdrNumABlks = 0;

  /* vol.mdb.drXTFlSize */
  /* vol.mdb.drCTFlSize */

  /* vol.mdb.drXTExtRec[0..2] */
  /* vol.mdb.drCTExtRec[0..2] */

  vol.flags |= HFS_VOL_UPDATE_MDB | HFS_VOL_UPDATE_ALTMDB;

  /* initialize volume bitmap */

  vol.vbm = ALLOC(block, vol.vbmsz);
  if (vol.vbm == 0)
    ERROR(ENOMEM, 0);

  memset(vol.vbm, 0, vol.vbmsz << HFS_BLOCKSZ_BITS);

  vol.flags |= HFS_VOL_UPDATE_VBM;

  /* perform initial bad block sparing */

  if (nbadblocks > 0)
    {
      if (nbadblocks * 4 > vol.vlen)
	ERROR(EINVAL, "volume contains too many bad blocks");

      badalloc = ALLOC(unsigned int, nbadblocks);
      if (badalloc == 0)
	ERROR(ENOMEM, 0);

      if (vol.mdb.drNmAlBlks == 1594)
	vol.mdb.drFreeBks = --vol.mdb.drNmAlBlks;

      for (i = 0; i < nbadblocks; ++i)
	{
	  unsigned long bnum;
	  unsigned int anum;

	  bnum = badblocks[i];

	  if (bnum < vol.mdb.drAlBlSt || bnum == vol.vlen - 2)
	    ERROR(EINVAL, "can't spare critical bad block");
	  else if (bnum >= vol.vlen)
	    ERROR(EINVAL, "bad block not in volume");

	  anum = (bnum - vol.mdb.drAlBlSt) / vol.lpa;

	  if (anum < vol.mdb.drNmAlBlks)
	    BMSET(vol.vbm, anum);

	  badalloc[i] = anum;
	}

      vol.mdb.drAtrb |= HFS_ATRB_BBSPARED;
    }

  /* create extents overflow file */

  n_init(&ext->hdrnd, ext, ndHdrNode, 0);

  ext->hdrnd.nnum       = 0;
  ext->hdrnd.nd.ndNRecs = 3;
  ext->hdrnd.roff[1]    = 0x078;
  ext->hdrnd.roff[2]    = 0x0f8;
  ext->hdrnd.roff[3]    = 0x1f8;

  memset(HFS_NODEREC(ext->hdrnd, 1), 0, 128);

  ext->hdr.bthDepth    = 0;
  ext->hdr.bthRoot     = 0;
  ext->hdr.bthNRecs    = 0;
  ext->hdr.bthFNode    = 0;
  ext->hdr.bthLNode    = 0;
  ext->hdr.bthNodeSize = HFS_BLOCKSZ;
  ext->hdr.bthKeyLen   = 0x07;
  ext->hdr.bthNNodes   = 0;
  ext->hdr.bthFree     = 0;
  for (i = 0; i < 76; ++i)
    ext->hdr.bthResv[i] = 0;

  ext->map = ALLOC(byte, HFS_MAP1SZ);
  if (ext->map == 0)
    ERROR(ENOMEM, 0);

  memset(ext->map, 0, HFS_MAP1SZ);
  BMSET(ext->map, 0);

  ext->mapsz = HFS_MAP1SZ;
  ext->flags = HFS_BT_UPDATE_HDR;

  /* create catalog file */

  n_init(&cat->hdrnd, cat, ndHdrNode, 0);

  cat->hdrnd.nnum       = 0;
  cat->hdrnd.nd.ndNRecs = 3;
  cat->hdrnd.roff[1]    = 0x078;
  cat->hdrnd.roff[2]    = 0x0f8;
  cat->hdrnd.roff[3]    = 0x1f8;

  memset(HFS_NODEREC(cat->hdrnd, 1), 0, 128);

  cat->hdr.bthDepth    = 0;
  cat->hdr.bthRoot     = 0;
  cat->hdr.bthNRecs    = 0;
  cat->hdr.bthFNode    = 0;
  cat->hdr.bthLNode    = 0;
  cat->hdr.bthNodeSize = HFS_BLOCKSZ;
  cat->hdr.bthKeyLen   = 0x25;
  cat->hdr.bthNNodes   = 0;
  cat->hdr.bthFree     = 0;
  for (i = 0; i < 76; ++i)
    cat->hdr.bthResv[i] = 0;

  cat->map = ALLOC(byte, HFS_MAP1SZ);
  if (cat->map == 0)
    ERROR(ENOMEM, 0);

  memset(cat->map, 0, HFS_MAP1SZ);
  BMSET(cat->map, 0);

  cat->mapsz = HFS_MAP1SZ;
  cat->flags = HFS_BT_UPDATE_HDR;

  /* allocate space for header nodes (and initial extents) */

  if (bt_space(ext, 1) == -1 ||
      bt_space(cat, 1) == -1)
    goto fail;

  --ext->hdr.bthFree;
  --cat->hdr.bthFree;

  /* create extent records for bad blocks */

  if (nbadblocks > 0)
    {
      hfsfile bbfile;
      ExtDescriptor extent;
      ExtDataRec *extrec;
      ExtKeyRec key;
      byte record[HFS_MAX_EXTRECLEN];
      unsigned int reclen;

      f_init(&bbfile, &vol, HFS_CNID_BADALLOC, "bad blocks");

      qsort(badalloc, nbadblocks, sizeof(*badalloc),
	    (int (*)(const void *, const void *)) compare);

      for (i = 0; i < nbadblocks; ++i)
	{
	  if (i == 0 || badalloc[i] != extent.xdrStABN)
	    {
	      extent.xdrStABN    = badalloc[i];
	      extent.xdrNumABlks = 1;

	      if (extent.xdrStABN < vol.mdb.drNmAlBlks &&
		  f_addextent(&bbfile, &extent) == -1)
		goto fail;
	    }
	}

      /* flush local extents into extents overflow file */

      f_getptrs(&bbfile, &extrec, 0, 0);

      r_makeextkey(&key, bbfile.fork, bbfile.cat.u.fil.filFlNum, 0);
      r_packextrec(&key, extrec, record, &reclen);

      if (bt_insert(&vol.ext, record, reclen) == -1)
	goto fail;
    }

  vol.flags |= HFS_VOL_MOUNTED;

  /* create root directory */

  if (v_mkdir(&vol, HFS_CNID_ROOTPAR, vname) == -1)
    goto fail;

  vol.mdb.drNxtCNID = 16;  /* first CNID not reserved by Apple */

  /* write boot blocks */

  if (m_zerobb(&vol) == -1)
    goto fail;

  /* zero other unused space, if requested */

  if (vol.flags & HFS_OPT_ZERO)
    {
      block b;
      unsigned long bnum;

      memset(&b, 0, sizeof(b));

      /* between MDB and VBM (never) */

      for (bnum = 3; bnum < vol.mdb.drVBMSt; ++bnum)
	b_writelb(&vol, bnum, &b);

      /* between VBM and first allocation block (sometimes if HFS_OPT_2048) */

      for (bnum = vol.mdb.drVBMSt + vol.vbmsz; bnum < vol.mdb.drAlBlSt; ++bnum)
	b_writelb(&vol, bnum, &b);

      /* between last allocation block and alternate MDB (sometimes) */

      for (bnum = vol.mdb.drAlBlSt + vol.mdb.drNmAlBlks * vol.lpa;
	   bnum < vol.vlen - 2; ++bnum)
	b_writelb(&vol, bnum, &b);

      /* final block (always) */

      b_writelb(&vol, vol.vlen - 1, &b);
    }

  /* flush remaining state and close volume */

  if (v_close(&vol) == -1)
    goto fail;

  FREE(badalloc);

  return 0;

fail:
  v_close(&vol);

  FREE(badalloc);

  return -1;
}
