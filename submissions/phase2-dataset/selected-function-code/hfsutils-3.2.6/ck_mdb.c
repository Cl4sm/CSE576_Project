{
  MDB *mdb = &vol->mdb;
  time_t now;

  printf("*** Checking volume MDB\n");

  if (VERBOSE)
    {
      int flag;

      printf("  drSigWord      = 0x%04x\n", mdb->drSigWord);

      printf("  drCrDate       = %s\n", mctime(mdb->drCrDate));
      printf("  drLsMod        = %s\n", mctime(mdb->drLsMod));

      printf("  drAtrb         =");
      flag = 0;
      if (mdb->drAtrb & HFS_ATRB_BUSY)
	{
	  printf("%s BUSY", flag ? " |" : "");
	  flag = 1;
	}
      if (mdb->drAtrb & HFS_ATRB_HLOCKED)
	{
	  printf("%s HLOCKED", flag ? " |" : "");
	  flag = 1;
	}
      if (mdb->drAtrb & HFS_ATRB_UMOUNTED)
	{
	  printf("%s UMOUNTED", flag ? " |" : "");
	  flag = 1;
	}
      if (mdb->drAtrb & HFS_ATRB_BBSPARED)
	{
	  printf("%s BBSPARED", flag ? " |" : "");
	  flag = 1;
	}
      if (mdb->drAtrb & HFS_ATRB_BVINCONSIS)
	{
	  printf("%s BVINCONSIS", flag ? " |" : "");
	  flag = 1;
	}
      if (mdb->drAtrb & HFS_ATRB_COPYPROT)
	{
	  printf("%s COPYPROT", flag ? " |" : "");
	  flag = 1;
	}
      if (mdb->drAtrb & HFS_ATRB_SLOCKED)
	{
	  printf("%s SLOCKED", flag ? " |" : "");
	  flag = 1;
	}

      if (flag == 0)
	printf(" 0");

      printf("\n");

      printf("  drNmFls        = %u\n", mdb->drNmFls);
      printf("  drVBMSt        = %u\n", mdb->drVBMSt);
      printf("  drAllocPtr     = %u\n", mdb->drAllocPtr);

      printf("  drNmAlBlks     = %u\n", mdb->drNmAlBlks);
      printf("  drAlBlkSiz     = %lu\n", mdb->drAlBlkSiz);
      printf("  drClpSiz       = %lu\n", mdb->drClpSiz);
      printf("  drAlBlSt       = %u\n", mdb->drAlBlSt);

      printf("  drNxtCNID      = %lu\n", mdb->drNxtCNID);
      printf("  drFreeBks      = %u\n", mdb->drFreeBks);
      printf("  drVN           = \"%s\"\n", mdb->drVN);

      printf("  drVolBkUp      = %s\n", mctime(mdb->drVolBkUp));
      printf("  drVSeqNum      = %u\n", mdb->drVSeqNum);
      printf("  drWrCnt        = %lu\n", mdb->drWrCnt);

      printf("  drXTClpSiz     = %lu\n", mdb->drXTClpSiz);
      printf("  drCTClpSiz     = %lu\n", mdb->drCTClpSiz);

      printf("  drNmRtDirs     = %u\n", mdb->drNmRtDirs);
      printf("  drFilCnt       = %lu\n", mdb->drFilCnt);
      printf("  drDirCnt       = %lu\n", mdb->drDirCnt);

      /* drFndrInfo */

      printf("  drEmbedSigWord = 0x%04x\n", mdb->drEmbedSigWord);
      printf("  drEmbedExtent  = %s\n", extstr(&mdb->drEmbedExtent));

      printf("  drXTFlSize     = %lu\n", mdb->drXTFlSize);
      printf("  drXTExtRec     = %s\n", extrecstr(&mdb->drXTExtRec));

      printf("  drCTFlSize     = %lu\n", mdb->drCTFlSize);
      printf("  drCTExtRec     = %s\n", extrecstr(&mdb->drCTExtRec));
    }

  /* mdb->drSigWord */

  if (mdb->drSigWord != HFS_SIGWORD &&
      ask("Bad volume signature (0x%04x); should be 0x%04x",
	  mdb->drSigWord, HFS_SIGWORD))
    {
      mdb->drSigWord = HFS_SIGWORD;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  /* mdb->drCrDate */

  time(&now);

  if (mdb->drCrDate == 0 &&
      ask("Volume creation date is unset"))
    {
      mdb->drCrDate = d_mtime(now);
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  if (d_ltime(mdb->drCrDate) > now &&
      ask("Volume creation date is in the future (%s)", mctime(mdb->drCrDate)))
    {
      mdb->drCrDate = d_mtime(now);
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  /* mdb->drLsMod */

  if (mdb->drLsMod == 0 &&
      ask("Volume last modify date is unset"))
    {
      mdb->drLsMod = mdb->drCrDate;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  if (d_ltime(mdb->drLsMod) > now &&
      ask("Volume last modify date is in the future (%s)",
	  mctime(mdb->drLsMod)))
    {
      mdb->drLsMod = d_mtime(now);
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  if (mdb->drLsMod < mdb->drCrDate &&
      ask("Volume last modify date is before volume creation"))
    {
      mdb->drLsMod = mdb->drCrDate;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  /* mdb->drAtrb */
  /* mdb->drNmFls */

  /* mdb->drVBMSt */

  if (mdb->drVBMSt != 3 &&
      ask("Volume bitmap starts at unusual location (%u), not 3"))
    {
      mdb->drVBMSt = 3;
      vol->flags |= HFS_VOL_UPDATE_MDB;
    }

  /* mdb->drAllocPtr */
  /* ... */

  /* initialize other important fields */

  vol->lpa = vol->mdb.drAlBlkSiz >> HFS_BLOCKSZ_BITS;

  /* extents pseudo-file structs */

  vol->ext.f.cat.u.fil.filStBlk   = vol->mdb.drXTExtRec[0].xdrStABN;
  vol->ext.f.cat.u.fil.filLgLen   = vol->mdb.drXTFlSize;
  vol->ext.f.cat.u.fil.filPyLen   = vol->mdb.drXTFlSize;

  vol->ext.f.cat.u.fil.filCrDat   = vol->mdb.drCrDate;
  vol->ext.f.cat.u.fil.filMdDat   = vol->mdb.drLsMod;

  vol->ext.f.cat.u.fil.filClpSize = vol->mdb.drXTClpSiz;

  memcpy(&vol->ext.f.cat.u.fil.filExtRec,
	 &vol->mdb.drXTExtRec, sizeof(ExtDataRec));

  f_selectfork(&vol->ext.f, fkData);

  /* catalog pseudo-file structs */

  vol->cat.f.cat.u.fil.filStBlk   = vol->mdb.drCTExtRec[0].xdrStABN;
  vol->cat.f.cat.u.fil.filLgLen   = vol->mdb.drCTFlSize;
  vol->cat.f.cat.u.fil.filPyLen   = vol->mdb.drCTFlSize;

  vol->cat.f.cat.u.fil.filCrDat   = vol->mdb.drCrDate;
  vol->cat.f.cat.u.fil.filMdDat   = vol->mdb.drLsMod;

  vol->cat.f.cat.u.fil.filClpSize = vol->mdb.drCTClpSiz;

  memcpy(&vol->cat.f.cat.u.fil.filExtRec,
	 &vol->mdb.drCTExtRec, sizeof(ExtDataRec));

  f_selectfork(&vol->cat.f, fkData);

  return 0;
}
