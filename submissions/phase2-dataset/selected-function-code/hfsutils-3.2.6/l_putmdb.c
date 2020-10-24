int l_putmdb(hfsvol *vol, const MDB *mdb, int backup)
{
  block b;
  byte *ptr = b;
  int i;

  d_storesw(&ptr, mdb->drSigWord);
  d_storesl(&ptr, mdb->drCrDate);
  d_storesl(&ptr, mdb->drLsMod);
  d_storesw(&ptr, mdb->drAtrb);
  d_storeuw(&ptr, mdb->drNmFls);
  d_storeuw(&ptr, mdb->drVBMSt);
  d_storeuw(&ptr, mdb->drAllocPtr);
  d_storeuw(&ptr, mdb->drNmAlBlks);
  d_storeul(&ptr, mdb->drAlBlkSiz);
  d_storeul(&ptr, mdb->drClpSiz);
  d_storeuw(&ptr, mdb->drAlBlSt);
  d_storesl(&ptr, mdb->drNxtCNID);
  d_storeuw(&ptr, mdb->drFreeBks);

  d_storestr(&ptr, mdb->drVN, sizeof(mdb->drVN));

  ASSERT(ptr - b == 64);

  d_storesl(&ptr, mdb->drVolBkUp);
  d_storesw(&ptr, mdb->drVSeqNum);
  d_storeul(&ptr, mdb->drWrCnt);
  d_storeul(&ptr, mdb->drXTClpSiz);
  d_storeul(&ptr, mdb->drCTClpSiz);
  d_storeuw(&ptr, mdb->drNmRtDirs);
  d_storeul(&ptr, mdb->drFilCnt);
  d_storeul(&ptr, mdb->drDirCnt);

  for (i = 0; i < 8; ++i)
    d_storesl(&ptr, mdb->drFndrInfo[i]);

  ASSERT(ptr - b == 124);

  d_storeuw(&ptr, mdb->drEmbedSigWord);
  d_storeuw(&ptr, mdb->drEmbedExtent.xdrStABN);
  d_storeuw(&ptr, mdb->drEmbedExtent.xdrNumABlks);

  d_storeul(&ptr, mdb->drXTFlSize);

  for (i = 0; i < 3; ++i)
    {
      d_storeuw(&ptr, mdb->drXTExtRec[i].xdrStABN);
      d_storeuw(&ptr, mdb->drXTExtRec[i].xdrNumABlks);
    }

  ASSERT(ptr - b == 146);

  d_storeul(&ptr, mdb->drCTFlSize);

  for (i = 0; i < 3; ++i)
    {
      d_storeuw(&ptr, mdb->drCTExtRec[i].xdrStABN);
      d_storeuw(&ptr, mdb->drCTExtRec[i].xdrNumABlks);
    }

  ASSERT(ptr - b == 162);

  memset(ptr, 0, HFS_BLOCKSZ - (ptr - b));

  if (b_writelb(vol, 2, &b) == -1 ||
      (backup && b_writelb(vol, vol->vlen - 2, &b) == -1))
    goto fail;

  return 0;

fail:
  return -1;
}
